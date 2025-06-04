#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetPtr.h"

namespace Copper {

    class VertexArray;

    class Transform;
    class Camera;
    class Light;

    class Material;
    class Mesh;

    namespace Renderer {

        void Initialize();
        void Restart();

        void StartBatch();
        void LoadBatch();
        void RenderBatch();
        void NewBatch();

        void RenderLines();

        void AddMesh(const AssetPtr<Mesh>& mesh, Transform* transform, const AssetPtr<Material>& material);

        void AddLine(const Vector3& start, const Vector3& end, const Color& color);
        void AddLine(const Vector3& start, const Vector3& end, const Color& color, Transform* transform);
        void AddCube(const Vector3& centre, const Vector3& size, const Color& color, Transform* transform);

        void AddLight(Light* light);
        void ClearLights();

        void SetCamera(Camera* cam);

        void SetWireframe(bool value = true);
        void SetShaderPath(const fs::path &vertexPath, const fs::path& fragmentPath);

        bool GetWireframe();

        bool IsFirstFrame();

        Color& AmbientColor();
        Vector3& AmbientDirection();

        Color& SkyboxColor();

    }

    // A very low Level wrapper for the renderer api
    // Source is in the Platform/<API> for the current API
    namespace RendererAPI {

        void Initialize();

        void ClearColor(const Color& color);
        void ResizeViewport(const UVector2I& size);

        void Render(VertexArray* vao, uint32 count, Light** lights, uint32 lightCount, AssetPtr<Material>* materials, uint32 materialCount);
        void RenderLines(VertexArray* vao, uint32 vertexCount);
        void EndFrame();

        void SetCamera(Camera* cam);

        void SetWireframe(bool value);
        void SetShaderPath(const fs::path& vertexPath, const fs::path& fragmentPath);

    }

}
