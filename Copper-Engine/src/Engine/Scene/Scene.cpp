#include "cupch.h"
#include "Scene.h"

#include "Engine/Events/RegistryEvent.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/Field.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/Classes.h"

#include "Engine/Physics/Raycast.h"

#include "Engine/Input/Popup.h"

#include "Engine/YAMLOverloads/Everything.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define MANAGED_REFERENCE_ADD(cID, klass) case cID: Scripting::CreateManagedReference((klass*) event->component, Scripting::Class::klass); break;

namespace Copper {

    namespace Renderer {

        extern void StartFrame();

        extern void Render(Camera* cam, bool gizmos);

    }

    uint32 cCounter = 0;

    std::unordered_map<uint32, std::function<bool(const YAML::Node&, Scene*)>> oldDeserializeFunctions;

    Scene::~Scene() {

        CUP_FUNCTION();

        Renderer::ClearLights();

    }

    void Scene::Initialize() {

        CUP_FUNCTION();

        if (m_initialized) return;

        InitializePhysics();

        m_initialized = true;

    }
    void Scene::Deinitialize() {

        CUP_FUNCTION();

        if (!m_initialized) return;

        m_registry.Cleanup();
        DeinitializePhysics();

        m_initialized = false;
        m_cam = nullptr;

    }

    void Scene::Update(float deltaTime) {

        CUP_FUNCTION();

#ifdef CU_EDITOR
        if (!m_initialized) return;
#else
        CU_ASSERT(m_initialized, "Scene is not initialized!");
#endif

        Renderer::StartFrame();

        IN_RUNTIME(UpdatePhysics(deltaTime));

        CUP_START_FRAME("ECS Update");

        for (InternalEntity* entity : EntityView(this)) {

            IN_RUNTIME(RuntimeUpdateEntity(entity, deltaTime));

            if (Light* lightComponent = entity->GetComponent<Light>())
                Renderer::AddLight(lightComponent);
            if (Camera* cameraComponent = entity->GetComponent<Camera>()) {

                Renderer::SetCamera(cameraComponent);
                m_cam = cameraComponent;

            }
            if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>())
                Renderer::AddMesh(renderer->mesh, entity->m_transform, renderer->material);

            if (Collider* collider = entity->GetComponent<Collider>())
                Renderer::AddCube(collider->GetCenter(), Vector3::one, Color::red, entity->m_transform);

        }

        CUP_END_FRAME();

        Renderer::LoadBatch();
        if (m_cam != nullptr)
            Renderer::RenderBatch();

    }
    void Scene::RuntimeUpdateEntity(InternalEntity* entity, float deltaTIme) {

        CUP_FUNCTION();

        if (RigidBody* rb = entity->GetComponent<RigidBody>())
            rb->UpdatePositionAndRotation();

        if (ScriptComponent* script = entity->GetComponent<ScriptComponent>())
            script->Update();

    }

    void Scene::Render(Camera* cam, bool gizmos) {

        CUP_FUNCTION();
        Renderer::Render(cam, gizmos);

    }

    bool Scene::EntityCreated(const Event& e) {

        CUP_FUNCTION();

        using namespace Scripting;

        EntityEvent* event = (EntityEvent*) &e;

        CreateManagedReference((void*) (uint64) event->entity->m_id, Class::Entity);
        CreateManagedReference(event->entity->m_transform, Class::Transform);

        return true;

    }
    bool Scene::EntityRemoved(const Event& e) {

        CUP_FUNCTION();

        using namespace Scripting;

        EntityEvent* event = (EntityEvent*) &e;

        RemoveManagedReference((void*) (uint64) event->entity->m_id);
        RemoveManagedReference(event->entity->m_transform);

        return true;

    }

    bool Scene::ComponentAdded(const Event& e) {

        CUP_FUNCTION();

        ComponentEvent* event = (ComponentEvent*) &e;

        switch (event->componentID) {

            MANAGED_REFERENCE_ADD(CAMERA_CID, Camera);
            MANAGED_REFERENCE_ADD(LIGHT_CID, Light);

            MANAGED_REFERENCE_ADD(RIGIDBODY_CID, RigidBody);
            MANAGED_REFERENCE_ADD(BOX_COLLIDER_CID, BoxCollider);
            MANAGED_REFERENCE_ADD(SPHERE_COLLIDER_CID, SphereCollider);
            MANAGED_REFERENCE_ADD(CAPSULE_COLLIDER_CID, CapsuleCollider);

        }

        return true;

    }
    bool Scene::ComponentRemoved(const Event& e) {

        CUP_FUNCTION();

        ComponentEvent* event = (ComponentEvent*) &e;
        Scripting::RemoveManagedReference(event->component);

        return true;

    }

}
