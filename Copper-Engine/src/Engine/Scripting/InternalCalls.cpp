#include "cupch.h"

#include "Engine/Scripting/InternalCalls/Components/BoxCollider.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Camera.cs.h"
#include "Engine/Scripting/InternalCalls/Components/CapsuleCollider.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Collider.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Component.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Light.cs.h"
#include "Engine/Scripting/InternalCalls/Components/MeshRenderer.cs.h"
#include "Engine/Scripting/InternalCalls/Components/RigidBody.cs.h"
#include "Engine/Scripting/InternalCalls/Components/SphereCollider.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Transform.cs.h"
#include "Engine/Scripting/InternalCalls/Core/Entity.cs.h"
#include "Engine/Scripting/InternalCalls/Core/Game.cs.h"
#include "Engine/Scripting/InternalCalls/Core/UUID.cs.h"
#include "Engine/Scripting/InternalCalls/Editor/Editor.cs.h"
#include "Engine/Scripting/InternalCalls/Input/Cursor.cs.h"
#include "Engine/Scripting/InternalCalls/Input/Input.cs.h"
#include "Engine/Scripting/InternalCalls/Math/Quaternion.cs.h"
#include "Engine/Scripting/InternalCalls/Physics/Raycast.cs.h"
#include "Engine/Scripting/InternalCalls/Renderer/Material.cs.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting {

    void SetupInternalCalls() {

        CUP_FUNCTION();

        Entity::Initialize();

        mono_add_internal_call("Copper.BoxCollider::get_size", (void*) BoxCollider::GetSize);
        mono_add_internal_call("Copper.BoxCollider::set_size", (void*) BoxCollider::SetSize);
        mono_add_internal_call("Copper.Camera::get_fov", (void*) Camera::GetFov);
        mono_add_internal_call("Copper.Camera::set_fov", (void*) Camera::SetFov);
        mono_add_internal_call("Copper.Camera::get_nearPlane", (void*) Camera::GetNearPlane);
        mono_add_internal_call("Copper.Camera::set_nearPlane", (void*) Camera::SetNearPlane);
        mono_add_internal_call("Copper.Camera::get_farPlane", (void*) Camera::GetFarPlane);
        mono_add_internal_call("Copper.Camera::set_farPlane", (void*) Camera::SetFarPlane);
        mono_add_internal_call("Copper.CapsuleCollider::get_radius", (void*) CapsuleCollider::GetRadius);
        mono_add_internal_call("Copper.CapsuleCollider::set_radius", (void*) CapsuleCollider::SetRadius);
        mono_add_internal_call("Copper.CapsuleCollider::get_height", (void*) CapsuleCollider::GetHeight);
        mono_add_internal_call("Copper.CapsuleCollider::set_height", (void*) CapsuleCollider::SetHeight);
        mono_add_internal_call("Copper.Collider::get_type", (void*) Collider::GetType);
        mono_add_internal_call("Copper.Collider::get_rigidBody", (void*) Collider::GetRigidBody);
        mono_add_internal_call("Copper.Collider::get_trigger", (void*) Collider::GetTrigger);
        mono_add_internal_call("Copper.Collider::set_trigger", (void*) Collider::SetTrigger);
        mono_add_internal_call("Copper.Collider::get_center", (void*) Collider::GetCenter);
        mono_add_internal_call("Copper.Collider::set_center", (void*) Collider::SetCenter);
        mono_add_internal_call("Copper.Component::get_entity", (void*) Component::GetEntity);
        mono_add_internal_call("Copper.Component::get_transform", (void*) Component::GetTransform);
        mono_add_internal_call("Copper.Light::get_color", (void*) Light::GetColor);
        mono_add_internal_call("Copper.Light::set_color", (void*) Light::SetColor);
        mono_add_internal_call("Copper.Light::get_type", (void*) Light::GetType);
        mono_add_internal_call("Copper.Light::set_type", (void*) Light::SetType);
        mono_add_internal_call("Copper.Light::get_intensity", (void*) Light::GetIntensity);
        mono_add_internal_call("Copper.Light::set_intensity", (void*) Light::SetIntensity);
        mono_add_internal_call("Copper.MeshRenderer::get_material", (void*) MeshRenderer::GetMaterial);
        mono_add_internal_call("Copper.MeshRenderer::set_material", (void*) MeshRenderer::SetMaterial);
        mono_add_internal_call("Copper.RigidBody::get_mass", (void*) RigidBody::GetMass);
        mono_add_internal_call("Copper.RigidBody::set_mass", (void*) RigidBody::SetMass);
        mono_add_internal_call("Copper.RigidBody::get_isStatic", (void*) RigidBody::GetIsStatic);
        mono_add_internal_call("Copper.RigidBody::set_isStatic", (void*) RigidBody::SetIsStatic);
        mono_add_internal_call("Copper.RigidBody::get_gravity", (void*) RigidBody::GetGravity);
        mono_add_internal_call("Copper.RigidBody::set_gravity", (void*) RigidBody::SetGravity);
        mono_add_internal_call("Copper.RigidBody::get_lockMask", (void*) RigidBody::GetLockMask);
        mono_add_internal_call("Copper.RigidBody::set_lockMask", (void*) RigidBody::SetLockMask);
        mono_add_internal_call("Copper.RigidBody::get_collider", (void*) RigidBody::GetCollider);
        mono_add_internal_call("Copper.RigidBody::Internal_AddForce", (void*) RigidBody::AddForce);
        mono_add_internal_call("Copper.RigidBody::Internal_AddTorque", (void*) RigidBody::AddTorque);
        mono_add_internal_call("Copper.SphereCollider::get_radius", (void*) SphereCollider::GetRadius);
        mono_add_internal_call("Copper.SphereCollider::set_radius", (void*) SphereCollider::SetRadius);
        mono_add_internal_call("Copper.Transform::get_position", (void*) Transform::GetPosition);
        mono_add_internal_call("Copper.Transform::set_position", (void*) Transform::SetPosition);
        mono_add_internal_call("Copper.Transform::get_scale", (void*) Transform::GetScale);
        mono_add_internal_call("Copper.Transform::set_scale", (void*) Transform::SetScale);
        mono_add_internal_call("Copper.Transform::get_globalPosition", (void*) Transform::GetGlobalPosition);
        mono_add_internal_call("Copper.Transform::set_globalPosition", (void*) Transform::SetGlobalPosition);
        mono_add_internal_call("Copper.Transform::get_globalScale", (void*) Transform::GetGlobalScale);
        mono_add_internal_call("Copper.Transform::set_globalScale", (void*) Transform::SetGlobalScale);
        mono_add_internal_call("Copper.Transform::get_forward", (void*) Transform::GetForward);
        mono_add_internal_call("Copper.Transform::get_right", (void*) Transform::GetRight);
        mono_add_internal_call("Copper.Transform::get_up", (void*) Transform::GetUp);
        mono_add_internal_call("Copper.Transform::get_parent", (void*) Transform::GetParent);
        mono_add_internal_call("Copper.Transform::set_parent", (void*) Transform::SetParent);
        mono_add_internal_call("Copper.Transform::get_childCount", (void*) Transform::GetChildCount);
        mono_add_internal_call("Copper.Transform::Internal_GetRotation", (void*) Transform::GetRotation);
        mono_add_internal_call("Copper.Transform::Internal_SetRotation", (void*) Transform::SetRotation);
        mono_add_internal_call("Copper.Transform::Internal_GetGlobalRotation", (void*) Transform::GetGlobalRotation);
        mono_add_internal_call("Copper.Transform::Internal_SetGlobalRotation", (void*) Transform::SetGlobalRotation);
        mono_add_internal_call("Copper.Transform::Internal_GetChild", (void*) Transform::GetChild);
        mono_add_internal_call("Copper.Transform::Internal_AddChild", (void*) Transform::AddChild);
        mono_add_internal_call("Copper.Transform::Internal_RemoveChild", (void*) Transform::RemoveChild);
        mono_add_internal_call("Copper.Transform::Internal_RemoveChildTransform", (void*) Transform::RemoveChildTransform);
        mono_add_internal_call("Copper.Entity::get_name", (void*) Entity::GetName);
        mono_add_internal_call("Copper.Entity::set_name", (void*) Entity::SetName);
        mono_add_internal_call("Copper.Entity::get_transform", (void*) Entity::GetTransform);
        mono_add_internal_call("Copper.Entity::Internal_AddComponent", (void*) Entity::AddComponent);
        mono_add_internal_call("Copper.Entity::Internal_GetComponent", (void*) Entity::GetComponent);
        mono_add_internal_call("Copper.Entity::Internal_HasComponent", (void*) Entity::HasComponent);
        mono_add_internal_call("Copper.Entity::Internal_RemoveComponent", (void*) Entity::RemoveComponent);
        mono_add_internal_call("Copper.Game::get_deltaTime", (void*) Game::GetDeltaTime);
        mono_add_internal_call("Copper.UUID::Internal_Constructor", (void*) UUID::Constructor);
        mono_add_internal_call("Copper.UUID::Internal_GenerateUUID", (void*) UUID::GenerateUUID);
        mono_add_internal_call("Copper.UUID::Internal_SetBytes", (void*) UUID::SetBytes);
        mono_add_internal_call("Copper.UUID::Internal_SetString", (void*) UUID::SetString);
        mono_add_internal_call("Copper.UUID::Internal_ToString", (void*) UUID::ToString);
        mono_add_internal_call("Copper.UUID::Internal_Equals", (void*) UUID::Equals);
        mono_add_internal_call("Copper.UUID::Internal_LessThan", (void*) UUID::LessThan);
        mono_add_internal_call("Copper.UUID::Internal_Hash", (void*) UUID::Hash);
        mono_add_internal_call("Copper.Editor::Internal_EditorLog", (void*) Editor::EditorLog);
        mono_add_internal_call("Copper.Editor::Internal_EditorLogWarn", (void*) Editor::EditorLogWarn);
        mono_add_internal_call("Copper.Editor::Internal_EditorLogError", (void*) Editor::EditorLogError);
        mono_add_internal_call("Copper.Cursor::get_visible", (void*) Cursor::IsVisible);
        mono_add_internal_call("Copper.Cursor::set_visible", (void*) Cursor::SetVisible);
        mono_add_internal_call("Copper.Cursor::get_locked", (void*) Cursor::IsLocked);
        mono_add_internal_call("Copper.Cursor::set_locked", (void*) Cursor::SetLocked);
        mono_add_internal_call("Copper.Cursor::get_position", (void*) Cursor::GetPosition);
        mono_add_internal_call("Copper.Cursor::set_position", (void*) Cursor::SetPosition);
        mono_add_internal_call("Copper.Input::Internal_GetKeyState", (void*) Input::GetKeyState);
        mono_add_internal_call("Copper.Input::Internal_GetAxis", (void*) Input::GetAxis);
        mono_add_internal_call("Copper.Input::Internal_GetMouseAxis", (void*) Input::GetMouseAxis);
        mono_add_internal_call("Copper.Quaternion::Internal_ToEuler", (void*) Quaternion::ToEuler);
        mono_add_internal_call("Copper.Quaternion::Internal_FromEuler", (void*) Quaternion::FromEuler);
        mono_add_internal_call("Copper.Raycast::Internal_Fire", (void*) Raycast::Fire);
        mono_add_internal_call("Copper.Material::get_albedo", (void*) Material::GetAlbedo);
        mono_add_internal_call("Copper.Material::set_albedo", (void*) Material::SetAlbedo);
        mono_add_internal_call("Copper.Material::get_tiling", (void*) Material::GetTiling);
        mono_add_internal_call("Copper.Material::set_tiling", (void*) Material::SetTiling);

    }

}