using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Raycast {

        public Raycast(Vector3 origin, Vector3 direction, float maxDistance = 1000.0f, bool fire = true) {

            this.m_origin = origin;
            this.m_direction = direction;
            this.m_maxDistance = maxDistance;

            if (fire)
                Fire();

        }

        private Vector3 m_origin = Vector3.zero;
        private Vector3 m_direction = new Vector3(0.0f, 0.0f, -1.0f);
        private float m_maxDistance = 1000.0f;

        private Data m_hitData;

        public Data hitData { get => m_hitData; }

        public bool hit { get => m_hitData.hit; }

        public Vector3 position { get => m_hitData.position; }
        public Vector3 normal {  get => m_hitData.normal; }
        public float distance {  get => m_hitData.distance; }

        public Entity entity { get => m_hitData.entity; }
        
        public static bool Fire(Vector3 origin, Vector3 direction, out Data hitData, float maxDistance = 1000.0f) { return Internal_Fire(origin, direction, out hitData, maxDistance); }
        public bool Fire() { return Fire(m_origin, m_direction, out m_hitData, m_maxDistance); }

        public struct Data {

            public readonly bool hit;

            public readonly Vector3 position;
            public readonly Vector3 normal;
            public readonly float distance;

            private IntPtr m_unmanagedEntityPointer;
            public Entity entity { get { return Internal_DataGetEntity(m_unmanagedEntityPointer); } }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_Fire(Vector3 origin, Vector3 direction, out Data hitData, float maxDistance);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Entity Internal_DataGetEntity(IntPtr unmanagedPointer);

    }

}