using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class InternalFunctions {

        //Logging
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Log(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LogError(string msg);

        //Object
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetObject(int id, out Object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetObjectName(int id, out string name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetObjectName(int id, ref string name);

        //Components
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool HasComponent(int id, Type componentType);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetComponent(int id, Type componentType, Component component);

        //Transform
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformGetPosition(int id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetPosition(int id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformGetRotation(int id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetRotation(int id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformGetScale(int id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformSetScale(int id, ref Vector3 position);

    }

}