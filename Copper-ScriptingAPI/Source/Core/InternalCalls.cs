using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class InternalCalls {

        //======== Core ========
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint GetComponentPointer(uint eID, Type type);

        //======== Logging ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Log(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void LogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void LogError(string msg);

        //======== Input ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKey(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKeyDown(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKeyReleased(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float GetAxis(string axisName);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetCursorVisible(bool visible);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetCursorLocked(bool locked);


        //======== Entity ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string GetEntityName(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetEntityName(uint eID, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsEntityValid(uint eID);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Entity GetEntity(uint eID);

        //======== Components ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void AddComponent(uint eID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool GetComponent(uint eID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool HasComponent(uint eID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetComponentEID(Type type, Component component, uint eID);

        //======== Transform ========
        
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetPosition(uint eID, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetRotation(uint eID, out Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetScale(uint eID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetPosition(uint eID, Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetRotation(uint eID, Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetScale(uint eID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetForward(uint eID, out Vector3 forward);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetRight(uint eID, out Vector3 right);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetUp(uint eID, out Vector3 up);


        //======== Camera ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetFOV(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetNearPlane(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetFarPlane(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetFOV(uint eID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetNearPlane(uint eID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetFarPlane(uint eID, float value);

        //======== RigidBody ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool RigidBodyGetIsStatic(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool RigidBodyGetGravity(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void RigidBodySetIsStatic(uint eID, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void RigidBodySetGravity(uint eID, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float RigidBodyGetMass(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void RigidBodySetMass(uint eID, float value);


        //======== BoxCollider ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool BoxColliderGetTrigger(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void BoxColliderSetTrigger(uint eID, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void BoxColliderGetCenter(uint eID, out Vector3 center);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void BoxColliderGetSize(uint eID, out Vector3 size);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void BoxColliderSetCenter(uint eID, Vector3 value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void BoxColliderSetSize(uint eID, Vector3 value);

        //======== Quaternion ========

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Vector3 QuaternionEulerAngles(Quaternion quaternion);

    }

}