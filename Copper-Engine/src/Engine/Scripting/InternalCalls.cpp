#include "cupch.h"

#include "Engine/Physics/Raycast.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	void Component_Initialize();

	void Initialize() {

		Component_Initialize();

	}

#pragma region Function Declarations

	// Editor

	void EditorLog(MonoString* msg);
	void EditorLogWarn(MonoString* msg);
	void EditorLogError(MonoString* msg);

	// Game
	
	float GetDeltaTime();

	// Input

	bool IsKey(int32 keyCode);
	bool IsKeyDown(int32 keyCode);
	bool IsKeyReleased(int32 keyCode);

	float GetAxis(MonoString* axisName);

	void SetCursorVisible(bool visible);
	void SetCursorLocked(bool locked);

	// Quaternion

	Vector3 QuaternionToEulerAngles(Quaternion* quaternion);
	Quaternion QuaternionFromEulerAngles(Vector3* euler);

	// Entity

	MonoString* GetEntityName(uint32 eID);
	void SetEntityName(uint32 eID, MonoString* out);

	bool IsEntityValid(uint32 eID);

	MonoObject* GetEntity(uint32 eID);

	// Component

	void AddComponent(uint32 eID, MonoReflectionType* type, MonoObject* ret);
	bool GetComponent(uint32 eID, MonoReflectionType* type, MonoObject* ret);
	bool HasComponent(uint32 eID, MonoReflectionType* type);

	void SetComponentEID(MonoReflectionType* type, MonoObject* component, uint32 eID);

	void SetComponentPointer(MonoReflectionType* type, MonoObject* component, uint32 eID);

	// Transform

	void GetPosition(uint64 componentPointer, Vector3* out);
	void GetRotation(uint64 componentPointer, Quaternion* out);
	void GetScale(uint64 componentPointer, Vector3* out);

	void GetGlobalPosition(uint64 componentPointer, Vector3* out);
	void GetGlobalRotation(uint64 componentPointer, Quaternion* out);
	void GetGlobalScale(uint64 componentPointer, Vector3* out);

	void GetForward(uint64 componentPointer, Vector3* out);
	void GetRight(uint64 componentPointer, Vector3* out);
	void GetUp(uint64 componentPointer, Vector3* out);

	void SetPosition(uint64 componentPointer, Vector3* value);
	void SetRotation(uint64 componentPointer, Quaternion* value);
	void SetScale(uint64 componentPointer, Vector3* value);

	// Camera

	float CameraGetFOV(uint64 componentPointer);
	float CameraGetNearPlane(uint64 componentPointer);
	float CameraGetFarPlane(uint64 componentPointer);

	void CameraSetFOV(uint64 componentPointer, float value);
	void CameraSetNearPlane(uint64 componentPointer, float value);
	void CameraSetFarPlane(uint64 componentPointer, float value);

	// RigidBody

	bool RigidBodyGetIsStatic(uint64 componentPointer);
	bool RigidBodyGetGravity(uint64 componentPointer);
	float RigidBodyGetMass(uint64 componentPointer);

	void RigidBodySetIsStatic(uint64 componentPointer, bool value);
	void RigidBodySetGravity(uint64 componentPointer, bool value);
	void RigidBodySetMass(uint64 componentPointer, float value);

	void RigidBodyAddForce(uint64 componentPointer, Vector3* force, uint8_t mode);
	void RigidBodyAddTorque(uint64 componentPointer, Vector3* torque, uint8_t mode);

	// Colliders

	bool ColliderGetTrigger(uint64 componentPointer);
	void ColliderGetCenter(uint64 componentPointer, Vector3* out);

	void ColliderSetTrigger(uint64 componentPointer, bool value);
	void ColliderSetCenter(uint64 componentPointer, Vector3* value);

	void BoxColliderGetSize(uint64 componentPointer, Vector3* out);
	void BoxColliderSetSize(uint64 componentPointer, Vector3* value);

	float SphereColliderGetRadius(uint64 componentPointer);
	void SphereColliderSetRadius(uint64 componentPointer, float value);

	float CapsuleColliderGetRadius(uint64 componentPointer);
	float CapsuleColliderGetHeight(uint64 componentPointer);
	void CapsuleColliderSetRadius(uint64 componentPointer, float value);
	void CapsuleColliderSetHeight(uint64 componentPointer, float value);

	// Raycast

	bool RaycastFire(Vector3* origin, Vector3* direction, Raycast::Data* hitData, float distance);

	MonoObject* RaycastDataGetEntity(InternalEntity* entity);

#pragma endregion

	void SetupInternalCalls() {

		//======== Editor ========

		mono_add_internal_call("Copper.Editor::Internal_Log", (void*) EditorLog);
		mono_add_internal_call("Copper.Editor::Internal_LogWarn", (void*) EditorLogWarn);
		mono_add_internal_call("Copper.Editor::Internal_LogError", (void*) EditorLogError);

		//======== Game ========

		mono_add_internal_call("Copper.Game::Internal_GetDeltaTime", (void*) GetDeltaTime);

		//======== Input ========

		mono_add_internal_call("Copper.Input::Internal_IsKey", (void*) IsKey);
		mono_add_internal_call("Copper.Input::Internal_IsKeyDown", (void*) IsKeyDown);
		mono_add_internal_call("Copper.Input::Internal_IsKeyReleased", (void*) IsKeyReleased);

		mono_add_internal_call("Copper.Input::Internal_GetAxis", (void*) GetAxis);

		mono_add_internal_call("Copper.Input::Internal_SetCursorVisible", (void*) SetCursorVisible);
		mono_add_internal_call("Copper.Input::Internal_SetCursorLocked", (void*) SetCursorLocked);

		//======== Quaternion ========

		mono_add_internal_call("Copper.Quaternion::Internal_ToEuler", (void*) QuaternionToEulerAngles);
		mono_add_internal_call("Copper.Quaternion::Internal_FromEuler", (void*) QuaternionFromEulerAngles);

		//======== Entity ========

		mono_add_internal_call("Copper.Entity::Internal_GetEntityName", (void*) GetEntityName);
		mono_add_internal_call("Copper.Entity::Internal_SetEntityName", (void*) SetEntityName);

		mono_add_internal_call("Copper.Entity::Internal_IsEntityValid", (void*) IsEntityValid);

		mono_add_internal_call("Copper.Entity::Internal_GetEntity", (void*) GetEntity);

		//======== Component ========

		mono_add_internal_call("Copper.Component::Internal_AddComponent", (void*) AddComponent);
		mono_add_internal_call("Copper.Component::Internal_GetComponent", (void*) GetComponent);
		mono_add_internal_call("Copper.Component::Internal_HasComponent", (void*) HasComponent);

		mono_add_internal_call("Copper.Component::Internal_SetComponentEID", (void*) SetComponentEID);

		mono_add_internal_call("Copper.Component::Internal_SetComponentPointer", (void*) SetComponentPointer);

		//======== Transform ========

		mono_add_internal_call("Copper.Transform::Internal_GetPosition", (void*) GetPosition);
		mono_add_internal_call("Copper.Transform::Internal_GetRotation", (void*) GetRotation);
		mono_add_internal_call("Copper.Transform::Internal_GetScale", (void*) GetScale);
		
		mono_add_internal_call("Copper.Transform::Internal_GetGlobalPosition", (void*) GetGlobalPosition);
		mono_add_internal_call("Copper.Transform::Internal_GetGlobalRotation", (void*) GetGlobalRotation);
		mono_add_internal_call("Copper.Transform::Internal_GetGlobalScale", (void*) GetGlobalScale);

		mono_add_internal_call("Copper.Transform::Internal_GetForward", (void*) GetForward);
		mono_add_internal_call("Copper.Transform::Internal_GetRight", (void*) GetRight);
		mono_add_internal_call("Copper.Transform::Internal_GetUp", (void*) GetUp);

		mono_add_internal_call("Copper.Transform::Internal_SetPosition", (void*) SetPosition);
		mono_add_internal_call("Copper.Transform::Internal_SetRotation", (void*) SetRotation);
		mono_add_internal_call("Copper.Transform::Internal_SetScale", (void*) SetScale);

		//======== Camera ========

		mono_add_internal_call("Copper.Camera::Internal_GetFOV", (void*) CameraGetFOV);
		mono_add_internal_call("Copper.Camera::Internal_GetNearPlane", (void*) CameraGetNearPlane);
		mono_add_internal_call("Copper.Camera::Internal_GetFarPlane", (void*) CameraGetFarPlane);

		mono_add_internal_call("Copper.Camera::Internal_SetFOV", (void*) CameraSetFOV);
		mono_add_internal_call("Copper.Camera::Internal_SetNearPlane", (void*) CameraSetNearPlane);
		mono_add_internal_call("Copper.Camera::Internal_SetFarPlane", (void*) CameraSetFarPlane);

		//======== RigidBody ========

		mono_add_internal_call("Copper.RigidBody::Internal_GetIsStatic", (void*) RigidBodyGetIsStatic);
		mono_add_internal_call("Copper.RigidBody::Internal_GetGravity", (void*) RigidBodyGetGravity);
		mono_add_internal_call("Copper.RigidBody::Internal_GetMass", (void*) RigidBodyGetMass);

		mono_add_internal_call("Copper.RigidBody::Internal_SetIsStatic", (void*) RigidBodySetIsStatic);
		mono_add_internal_call("Copper.RigidBody::Internal_SetGravity", (void*) RigidBodySetGravity);
		mono_add_internal_call("Copper.RigidBody::Internal_SetMass", (void*) RigidBodySetMass);

		mono_add_internal_call("Copper.RigidBody::Internal_AddForce", (void*) RigidBodyAddForce);
		mono_add_internal_call("Copper.RigidBody::Internal_AddTorque", (void*) RigidBodyAddTorque);

		//======== Colliders ========

		mono_add_internal_call("Copper.Collider::Internal_GetTrigger", (void*) ColliderGetTrigger);
		mono_add_internal_call("Copper.Collider::Internal_GetCenter", (void*) ColliderGetCenter);

		mono_add_internal_call("Copper.Collider::Internal_SetTrigger", (void*) ColliderSetTrigger);
		mono_add_internal_call("Copper.Collider::Internal_SetCenter", (void*) ColliderSetCenter);

		mono_add_internal_call("Copper.BoxCollider::Internal_GetSize", (void*) BoxColliderGetSize);
		mono_add_internal_call("Copper.BoxCollider::Internal_SetSize", (void*) BoxColliderSetSize);

		mono_add_internal_call("Copper.SphereCollider::Internal_GetRadius", (void*) SphereColliderGetRadius);
		mono_add_internal_call("Copper.SphereCollider::Internal_SetRadius", (void*) SphereColliderSetRadius);

		mono_add_internal_call("Copper.CapsuleCollider::Internal_GetRadius", (void*) CapsuleColliderGetRadius);
		mono_add_internal_call("Copper.CapsuleCollider::Internal_GetRadius", (void*) CapsuleColliderGetHeight);

		mono_add_internal_call("Copper.CapsuleCollider::Internal_SetRadius", (void*) SphereColliderSetRadius);
		mono_add_internal_call("Copper.CapsuleCollider::Internal_SetRadius", (void*) CapsuleColliderSetHeight);

		//======== Raycast ========
		mono_add_internal_call("Copper.Raycast::Internal_Fire", (void*) RaycastFire);

		mono_add_internal_call("Copper.Raycast::Internal_DataGetEntity", (void*) RaycastDataGetEntity);

	}

}