#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Entity.h"

#ifdef CU_EDITOR
#define COMPONENT_FORWARD_DECL() namespace YAML { class Emitter; class Node; } namespace Editor {\
                                 class Project; class Properties;\
                                 void CopyToClipboard(Copper::InternalEntity*); namespace SceneSerializer {\
                                 void SerializeEntityComponents(Copper::InternalEntity*, YAML::Emitter&);\
                                 void DeserializeEntityComponents(Copper::InternalEntity*, const YAML::Node&);\
                                 void DeserializeEntityTransform(Copper::InternalEntity**, const YAML::Node&);} }
#define COMPONENT_FRIEND_CLASSES() friend class Scene; friend Editor::Project; friend Editor::Properties;\
                                   friend void Editor::SceneSerializer::SerializeEntityComponents(Copper::InternalEntity*, YAML::Emitter&);\
                                   friend void Editor::SceneSerializer::DeserializeEntityComponents(Copper::InternalEntity*, const YAML::Node&);\
                                   friend void Editor::SceneSerializer::DeserializeEntityTransform(Copper::InternalEntity**, const YAML::Node&);\
                                   friend void Editor::CopyToClipboard(Copper::InternalEntity*);
#elif
#define COMPONENT_FORWARD_DECL()
#define COMPONENT_FRIEND_CLASSES() friend class Scene
#endif

#ifdef CU_EDITOR
namespace Editor { class SceneCamera; }
#endif

namespace Copper {

    class Component {

        friend class Registry;
        friend class Scene;

#ifdef CU_EDITOR
        friend Editor::SceneCamera;
#endif

    public:
        Entity GetEntity() const { return m_entity; }
        class Transform* GetTransform() const { return m_transform; }

        operator bool() const { return m_valid; }

    private:
        Entity m_entity = nullptr;
        Transform* m_transform = nullptr;

        bool m_valid = false;

    };

}

#include "Engine/Components/Transform.h"
