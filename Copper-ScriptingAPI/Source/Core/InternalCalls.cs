﻿using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class InternalCalls {

        //Logging
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Log(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void LogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void LogError(string msg);

        //Input
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKey(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKeyDown(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool IsKeyReleased(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float GetAxis(string axisName);

        //Object
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static CopperObject GetObject(int objID);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string GetObjectName(int objID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetObjectName(int objID, string name);

        //Components
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void AddComponent(int objID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool GetComponent(int objID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool HasComponent(int objID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetComponentObjID(Type type, Component component, int objID);

        //Transform
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetPosition(int objID, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetRotation(int objID, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetScale(int objID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetPosition(int objID, Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetRotation(int objID, Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SetScale(int objID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetForward(int objID, out Vector3 forward);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetBack(int objID, out Vector3 back);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetRight(int objID, out Vector3 right);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetLeft(int objID, out Vector3 left);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetUp(int objID, out Vector3 up);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void GetDown(int objID, out Vector3 down);


        //Camera
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetFOV(int objID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetNearPlane(int objID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float CameraGetFarPlane(int objID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetFOV(int objID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetNearPlane(int objID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void CameraSetFarPlane(int objID, float value);

    }

}