#include "cupch.h"

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

	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyReleased(int keyCode);

	float GetAxis(MonoString* axisName);

	void SetCursorVisible(bool visible);
	void SetCursorLocked(bool locked);

	// Quaternion

	Vector3 QuaternionToEulerAngles(Quaternion* quaternion);
	Quaternion QuaternionFromEulerAngles(Vector3* euler);

	// Entity

	MonoString* GetEntityName(uint32_t eID);
	void SetEntityName(uint32_t eID, MonoString* out);

	bool IsEntityValid(uint32_t eID);

	MonoObject* GetEntity(uint32_t eID);

	// Component

	void AddComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret);
	bool GetComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret);
	bool HasComponent(uint32_t eID, MonoReflectionType* type);

	void SetComponentEID(MonoReflectionType* type, MonoObject* component, uint32_t eID);

	void SetComponentPointer(MonoReflectionType* type, MonoObject* component, uint32_t eID);

	// Transform

	void GetPosition(int64_t componentPointer, Vector3* out);
	void GetRotation(int64_t componentPointer, Quaternion* out);
	void GetScale(int64_t componentPointer, Vector3* out);

	void GetForward(int64_t componentPointer, Vector3* out);
	void GetRight(int64_t componentPointer, Vector3* out);
	void GetUp(int64_t componentPointer, Vector3* out);

	void SetPosition(int64_t componentPointer, Vector3* value);
	void SetRotation(int64_t componentPointer, Quaternion* value);
	void SetScale(int64_t componentPointer, Vector3* value);

	//Camera

	float CameraGetFOV(int64_t componentPointer);
	float CameraGetNearPlane(int64_t componentPointer);
	float CameraGetFarPlane(int64_t componentPointer);

	void CameraSetFOV(int64_t componentPointer, float value);
	void CameraSetNearPlane(int64_t componentPointer, float value);
	void CameraSetFarPlane(int64_t componentPointer, float value);

	//RigidBody

	bool RigidBodyGetIsStatic(int64_t componentPointer);
	bool RigidBodyGetGravity(int64_t componentPointer);
	float RigidBodyGetMass(int64_t componentPointer);

	void RigidBodySetIsStatic(int64_t componentPointer, bool value);
	void RigidBodySetGravity(int64_t componentPointer, bool value);
	void RigidBodySetMass(int64_t componentPointer, float value);

	void RigidBodyAddForce(int64_t componentPointer, Vector3* force, uint8_t mode);
	void RigidBodyAddTorque(int64_t componentPointer, Vector3* torque, uint8_t mode);

	// Colliders
	bool ColliderGetTrigger(int64_t componentPointer);
	void ColliderGetCenter(int64_t componentPointer, Vector3* out);

	void ColliderSetTrigger(int64_t componentPointer, bool value);
	void ColliderSetCenter(int64_t componentPointer, Vector3* value);

	void BoxColliderGetSize(int64_t componentPointer, Vector3* out);
	void BoxColliderSetSize(int64_t componentPointer, Vector3* value);

	float SphereColliderGetRadius(int64_t componentPointer);
	void SphereColliderSetRadius(int64_t componentPointer, float value);

	float CapsuleColliderGetRadius(int64_t componentPointer);
	float CapsuleColliderGetHeight(int64_t componentPointer);
	void CapsuleColliderSetRadius(int64_t componentPointer, float value);
	void CapsuleColliderSetHeight(int64_t componentPointer, float value);

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

	}

}