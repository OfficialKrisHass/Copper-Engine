#include "cupch.h"
#include "Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/AssetStorage/AssetPtr.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include "Engine/Components/Transform.h"
#include "Engine/Components/Light.h"

#include <GLM/ext/matrix_transform.hpp>

namespace Copper { extern void InitializePrimitives(); }

namespace Copper::Renderer {

    constexpr uint32 MaxVertices = 20'000;
    constexpr uint32 MaxIndices = (uint32) (MaxVertices * 1.5f);

    constexpr uint32 MaxMaterials = 16;

    constexpr uint32 MaxLines = 10'000;
    constexpr uint32 MaxLineVertices = MaxLines * 2;

    struct Vertex {

        Vector3 position;
        Vector3 color;
        Vector3 normal;

        Vector2 uv;
        uint32 materialIndex = 0;

#ifdef CU_EDITOR
        uint32 entityID = INVALID_ENTITY_ID;
#endif

    };
    struct LineVertex {

        Vector3 pos;
        Vector3 color;

    };

    struct RendererData {

        // 3D Geometry

        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;

        uint32 verticesCount = 0;
        uint32 indicesCount = 0;

        Vertex* vertices = new Vertex[MaxVertices];
        uint32* indices = new uint32[MaxIndices];

        // Materials

        MaterialAsset* materials = new MaterialAsset[MaxMaterials];
        uint32 materialCount = 0;

        MaterialAsset whiteMaterial;

        // Lights

        Light** lights = new Light*[MAX_LIGHTS];
        uint32 lightCount = 0;

        // Lines

        VertexArray lineVao;
        VertexBuffer lineVbo;

        uint32 lineVertexCount = 0;
        LineVertex* lineVertices = new LineVertex[MaxLineVertices];

        // Misc.

        uint32 drawCalls = 0;
        bool firstFrame = true;

        Color skyboxColor = Color(0.18f, 0.18f, 0.18f);

        Color ambientColor = Color::white;
        Vector3 ambientDirection = Vector3(-0.489834040f, 0.210472092f, 0.846028447f);

        bool wireframe = false;
        
    };

    RendererData data;

    uint32 GetMaterialIndex(const MaterialAsset& material);

    void Initialize() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineState::Initialization, "Initialize the Renderer");

        RendererAPI::Initialize();

        // 3D Geometry

        data.vao.Create(nullptr);
        
        data.vbo.Create(nullptr, MaxVertices * sizeof(Vertex), {

            ElementType::Vec3, // Position
            ElementType::Vec3, // Color
            ElementType::Vec3, // Normal

            ElementType::Vec2, // UV
            ElementType::UInt, // Material Index

#ifdef CU_EDITOR
            ElementType::UInt, // Entity ID
#endif

        });
        data.ibo.Create(nullptr, MaxIndices);

        data.vao.SetVertexBuffer(&data.vbo);
        data.vao.SetIndexBuffer(&data.ibo);
                
        data.vbo.Unbind();
        data.vao.Unbind();

        // Lines

        data.lineVao.Create(nullptr);

        data.lineVbo.Create(nullptr, MaxLineVertices * sizeof(LineVertex), {

            ElementType::Vec3, // Position
            ElementType::Vec3, // Color

        });
        data.lineVao.SetVertexBuffer(&data.lineVbo);

        data.lineVbo.Unbind();
        data.lineVao.Unbind();

        // Built in Assets

        data.whiteMaterial = AssetStorage::InsertAsset<Material>(UUID::GetNil());

        uint32 white = 0xffffffff;
        data.whiteMaterial->texture = AssetStorage::InsertAsset<Texture>(UUID::GetNil(), 1, 1, Texture::Format::RGBA, (uint8*)&white);

        data.materials[0] = data.whiteMaterial;
        data.materialCount = 1;

        InitializePrimitives();

        LogStatus("\tRenderer initialized.");

    }
    void Restart() {

        CUP_FUNCTION();

        data.firstFrame = true;

    }

    void StartFrame() {

        CUP_FUNCTION();

        data.drawCalls = 0;
        data.lightCount = 0;

        data.lineVertexCount = 0;

        RendererAPI::ClearColor(data.skyboxColor);

        StartBatch();

    }
    void EndFrame() {

        CUP_FUNCTION();

        RendererAPI::EndFrame();
        data.firstFrame = false;

    }

    void StartBatch() {

        CUP_FUNCTION();

        data.verticesCount = 0;
        data.indicesCount = 0;

        data.materialCount = 1;

    }
    void LoadBatch() {

        CUP_FUNCTION();

        if (data.verticesCount == 0 || data.indicesCount == 0) return;

        data.vbo.SetData((float*) data.vertices, data.verticesCount * sizeof(Vertex));
        data.ibo.SetData(data.indices, data.indicesCount);

    }
    void RenderBatch() {

        CUP_FUNCTION();

        if (data.verticesCount == 0 || data.indicesCount == 0) return;

        CUP_START_FRAME("Render Batch");

        RendererAPI::Render(&data.vao, data.indicesCount, data.lights, data.lightCount, data.materials, data.materialCount);
        data.drawCalls++;

        CUP_END_FRAME();

    }
    void NewBatch() {

        CUP_FUNCTION();

        LoadBatch();
        RenderBatch();

        StartBatch();

    }

    void RenderLines() {

        CUP_FUNCTION();

        if (data.lineVertexCount == 0) return;

        data.lineVbo.SetData((float*) data.lineVertices, data.lineVertexCount * sizeof(LineVertex));
        RendererAPI::RenderLines(&data.lineVao, data.lineVertexCount);

        data.drawCalls++;

    }

    void AddMesh(const MeshAsset& mesh, Transform* transform, const MaterialAsset& material) {

        CUP_FUNCTION();

        if (!mesh.IsValid()) return;

        CU_ASSERT(transform != nullptr, "Transform is nullptr!");

        const Matrix4& transformMat = transform->GetTransformMatrix();

        const uint32 indicesCount = (uint32) mesh->indices.size();
        const uint32 verticesCount = (uint32) mesh->vertices.size();

        const uint32 matIndex = GetMaterialIndex(material);
        CU_ASSERT(matIndex != MaxMaterials, "Could not get material index of material '{}'", material.AssetUUID().ToString());

        // Check if batch is full

        if (data.indicesCount + indicesCount > MaxIndices ||
            data.verticesCount + verticesCount > MaxVertices ||
            data.materialCount >= MaxMaterials) {

            NewBatch();

        }

        // Load Mesh Data

        for (uint32 i = 0; i < verticesCount; i++) {

            Vertex& vertex = data.vertices[data.verticesCount + i];

            vertex.position = transformMat * Vector4(mesh->vertices[i], 1.0f);
            vertex.normal = (Matrix3) transformMat * mesh->normals[i];
            vertex.color = mesh->colors[i];

            vertex.uv = mesh->uvs[i];
            vertex.materialIndex = matIndex;

#ifdef CU_EDITOR
            vertex.entityID = transform->GetEntity().ID();
#endif

        }
        for (uint32 i = 0; i < indicesCount; i++)
            data.indices[data.indicesCount + i] = mesh->indices[i] + data.verticesCount;

        data.verticesCount += verticesCount;
        data.indicesCount += indicesCount;

    }

    void AddLine(const Vector3& start, const Vector3& end, const Color& color) {

        CUP_FUNCTION();

        if (data.lineVertexCount >= MaxLineVertices) {

            LogError("Max amount of lines reached ({})", MaxLines);
            return;

        }

        LineVertex* vertex = &data.lineVertices[data.lineVertexCount];
        vertex->pos = start;
        vertex->color = color;

        vertex = &data.lineVertices[data.lineVertexCount + 1];
        vertex->pos = end;
        vertex->color = color;

        data.lineVertexCount += 2;

    }
    void AddLine(const Vector3& start, const Vector3& end, const Color& color, Transform* transform) {

        CUP_FUNCTION();

        AddLine(transform->GetTransformMatrix() * Vector4(start, 1.0f), transform->GetTransformMatrix() * Vector4(end, 1.0f), color);

    }
    void AddCube(const Vector3& centre, const Vector3& size, const Color& color, Transform* transform) {

        CUP_FUNCTION();

        const Vector3 half = size / 2.0f;

        // Bottom

        static const Vector3 v0 = Vector3(-half.x, -half.y, half.z);
        static const Vector3 v1 = Vector3(half.x, -half.y, half.z);
        static const Vector3 v2 = Vector3(half.x, -half.y, -half.z);
        static const Vector3 v3 = Vector3(-half.x, -half.y, -half.z);

        // Top

        static const Vector3 v4 = Vector3(-half.x, half.y, half.z);
        static const Vector3 v5 = Vector3(half.x, half.y, half.z);
        static const Vector3 v6 = Vector3(half.x, half.y, -half.z);
        static const Vector3 v7 = Vector3(-half.x, half.y, -half.z);

        // Lines

        AddLine(centre + v0, centre + v1, color, transform);
        AddLine(centre + v1, centre + v2, color, transform);
        AddLine(centre + v2, centre + v3, color, transform);
        AddLine(centre + v3, centre + v0, color, transform);

        AddLine(centre + v4, centre + v5, color, transform);
        AddLine(centre + v5, centre + v6, color, transform);
        AddLine(centre + v6, centre + v7, color, transform);
        AddLine(centre + v7, centre + v4, color, transform);

        AddLine(centre + v0, centre + v4, color, transform);
        AddLine(centre + v1, centre + v5, color, transform);
        AddLine(centre + v2, centre + v6, color, transform);
        AddLine(centre + v3, centre + v7, color, transform);

    }

    void AddLight(Light* light) {

        CUP_FUNCTION();

        CU_EDITOR_ASSERT_RETURN(data.lightCount < MAX_LIGHTS,, "Max amount of lights reached ({})", MAX_LIGHTS);

        data.lights[data.lightCount] = light;
        data.lightCount++;

    }
    void ClearLights() {

        CUP_FUNCTION();

        data.lightCount = 0;

    }

    // This function is called at the end of each frame to render the last batch.
    void Render(Camera* cam, bool gizmos) {

        CUP_FUNCTION();

        RendererAPI::SetCamera(cam);
        RendererAPI::Render(&data.vao, data.indicesCount, data.lights, data.lightCount, data.materials, data.materialCount);
        if (gizmos)
            Renderer::RenderLines();

    }

    void SetCamera(Camera* cam) {

        CUP_FUNCTION();

        RendererAPI::SetCamera(cam);

    }

    void SetWireframe(bool value) {

        CUP_FUNCTION();

        data.wireframe = value;
        RendererAPI::SetWireframe(value);
    
    }
    void SetShaderPath(const fs::path& vertexPath, const fs::path& fragmentPath) {
        
        CUP_FUNCTION();

        RendererAPI::SetShaderPath(vertexPath, fragmentPath);
        
    }

    bool GetWireframe() { return data.wireframe; }

    bool IsFirstFrame() { return data.firstFrame; }

    Color& AmbientColor() { return data.ambientColor; }
    Vector3& AmbientDirection() { return data.ambientDirection; }

    Color& SkyboxColor() { return data.skyboxColor; }

    uint32 GetMaterialIndex(const MaterialAsset& material) {

        if (!material.IsValid() || data.materialCount == MaxMaterials) return MaxMaterials;

        for (uint32 i = 1; i < data.materialCount; i++) {

            if (data.materials[i] != material) continue;
            return i;

        }

        data.materials[data.materialCount] = material;
        data.materialCount++;

        return data.materialCount - 1;

    }

}

namespace Copper {

    using namespace Renderer;

    const MaterialAsset& Material::WhiteMaterial() {

        CU_ASSERT(data.whiteMaterial, "White material is invalid");

        return data.whiteMaterial;

    }
    const TextureAsset& Texture::WhiteTexture() {

        CU_ASSERT(data.whiteMaterial, "White material is invalid");
        CU_ASSERT(data.whiteMaterial->texture, "White texture is invalid");

        return data.whiteMaterial->texture;

    }

}
