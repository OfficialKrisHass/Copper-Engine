using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

  [StructLayout(LayoutKind.Sequential)]
  [NativeClass("Engine/Scripting/InternalCalls/Physics/Raycast.cs.h")]
  public class Raycast {

    private Vector3 m_origin = Vector3.zero;
    private Vector3 m_direction = new Vector3(0.0f, 0.0f, -1.0f);
    private float m_dist = 1000.0f;

    public Data m_data;

    public Raycast(Vector3 origin, Vector3 direction, float maxDistance = 1000.0f, bool fire = true) {

      this.m_origin = origin;
      this.m_direction = direction;
      this.m_dist = maxDistance;

      if (!fire) return;
      Fire();

    }

    public static bool Fire(Vector3 origin, Vector3 direction, out Data data, float maxDistance = 1000.0f) {

      return Internal_Fire(ref origin, ref direction, out data, maxDistance);

    }
    public bool Fire() {

      return Fire(m_origin, m_direction, out m_data, m_dist);

    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    [NativeFunction("Fire")]
    internal extern static bool Internal_Fire(ref Vector3 origin, ref Vector3 direction, out Data data, float maxDistance);

    [StructLayout(LayoutKind.Sequential)]
    public struct Data {

      public bool hit;

      public Vector3 position;
      public Vector3 normal;
      public float distance;

      public Entity entity;

    }

  }

}
