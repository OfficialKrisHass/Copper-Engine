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

	// Transform

	void GetPosition(uint32_t eID, Vector3* out);
	void GetRotation(uint32_t eID, Quaternion* out);
	void GetScale(uint32_t eID, Vector3* out);
	void GetForward(uint32_t eID, Vector3* out);
	void GetRight(uint32_t eID, Vector3* out);
	void GetUp(uint32_t eID, Vector3* out);

	void SetPosition(uint32_t eID, Vector3* value);
	void SetRotation(uint32_t eID, Quaternion* value);
	void SetScale(uint32_t eID, Vector3* value);

	//Camera

	float CameraGetFOV(uint32_t eID);
	float CameraGetNearPlane(uint32_t eID);
	float CameraGetFarPlane(uint32_t eID);

	void CameraSetFOV(uint32_t eID, float value);
	void CameraSetNearPlane(uint32_t eID, float value);
	void CameraSetFarPlane(uint32_t eID, float value);

	//RigidBody

	bool RigidBodyGetIsStatic(uint32_t eID);
	bool RigidBodyGetGravity(uint32_t eID);
	float RigidBodyGetMass(uint32_t eID);

	void RigidBodySetIsStatic(uint32_t eID, bool value);
	void RigidBodySetGravity(uint32_t eID, bool value);
	void RigidBodySetMass(uint32_t eID, float value);

	void RigidBodyAddForce(uint32_t eID, Vector3* force, uint8_t mode);
	void RigidBodyAddTorque(uint32_t eID, Vector3* torque, uint8_t mode);

	// Box Collider

	bool BoxColliderGetTrigger(uint32_t eID);
	void BoxColliderGetCenter(uint32_t eID, Vector3* out);
	void BoxColliderGetSize(uint32_t eID, Vector3* out);

	void BoxColliderSetCenter(uint32_t eID, Vector3* value);
	void BoxColliderSetTrigger(uint32_t eID, bool value);
	void BoxColliderSetSize(uint32_t eID, Vector3* value);

#pragma endregion

	void SetupInternalCalls() {

		//======== Editor ========

		mono_add_internal_call("Copper.Editor::Internal_Log", (void*) EditorLog);
		mono_add_internal_call("Copper.Editor::Internal_LogWarn", (void*) EditorLogWarn);
		mono_add_internal_call("Copper.Editor::Internal_LogError", (void*) EditorLogError);

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

		//======== Components ========

		mono_add_internal_call("Copper.Component::Internal_AddComponent", (void*) AddComponent);
		mono_add_internal_call("Copper.Component::Internal_GetComponent", (void*) GetComponent);
		mono_add_internal_call("Copper.Component::Internal_HasComponent", (void*) HasComponent);

		mono_add_internal_call("Copper.Entity::Internal_SetComponentEID", (void*) SetComponentEID);

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

		//======== BoxCollider ========

		mono_add_internal_call("Copper.BoxCollider::Internal_GetTrigger", (void*) BoxColliderGetTrigger);
		mono_add_internal_call("Copper.BoxCollider::Internal_GetCenter", (void*) BoxColliderGetCenter);
		mono_add_internal_call("Copper.BoxCollider::Internal_GetSize", (void*) BoxColliderGetSize);

		mono_add_internal_call("Copper.BoxCollider::Internal_SetTrigger", (void*) BoxColliderSetTrigger);
		mono_add_internal_call("Copper.BoxCollider::Internal_SetCenter", (void*) BoxColliderSetCenter);
		mono_add_internal_call("Copper.BoxCollider::Internal_SetSize", (void*) BoxColliderSetSize);

	}

}