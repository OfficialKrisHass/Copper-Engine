using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Globalization;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Math/Quaternion.cs.h")]
    public struct Quaternion {
        
        public Quaternion(float w, float x, float y, float z) {

            this.w = w;
            this.x = x;
            this.y = y;
            this.z = z;

        }
        public Quaternion(float w, Vector3 v) {

            this.w = w;
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;

        }

        public Quaternion(Vector3 eulerAngles) { Internal_FromEuler(ref eulerAngles, out this); }
        public Quaternion(float eulerX, float eulerY, float eulerZ) : this(new Vector3(eulerX, eulerY, eulerZ)) {} 

        public float w;
        public float x;
        public float y;
        public float z;

        public Vector3 eulerAngles {

            get { return Internal_ToEuler(ref this); }
            set { Internal_FromEuler(ref value, out this); }

        }

        public readonly static Quaternion identity = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

        public static Quaternion operator+(Quaternion lhs, Quaternion rhs) { return new Quaternion(lhs.w + rhs.w, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
        public static Quaternion operator-(Quaternion lhs, Quaternion rhs) { return new Quaternion(lhs.w - rhs.w, lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
        public static Quaternion operator*(Quaternion lhs, Quaternion rhs) {

            return new Quaternion(
                    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
                    lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                    lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                    lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x);

        }

        public static Vector3 operator*(Quaternion rotation, Vector3 point) {

            Vector3 quat = new Vector3(rotation.x, rotation.y, rotation.z);
            Vector3 uv = quat.Cross(point);
            Vector3 uuv = quat.Cross(uv);

            return point + ((uv * rotation.w) + uuv) * 2.0f;

        }

        public static Quaternion operator*(Quaternion lhs, float rhs) { return new Quaternion(lhs.w * rhs, lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
        public static Quaternion operator/(Quaternion lhs, float rhs) {

            float div = 1.0f / rhs;
            return lhs * div;

        }

        public static bool operator==(Quaternion lhs, Quaternion rhs) { return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
        public static bool operator!=(Quaternion lhs, Quaternion rhs) { return !(lhs == rhs); }

        public float Dot(Quaternion other) { return (x * other.x + y * other.y) + (z * other.z + w * other.w); }
        public Quaternion Conjugate() { return new Quaternion(w, -x, -y, -z); }

        public float length { get { return (float) Math.Sqrt(Dot(this)); } }

        public void Normalize() { this = normalized; }
        public Quaternion normalized {

            get {

                float len = length;
                if (len <= 0.0f)
                    return identity;

                float tmp = 1.0f / len;
                return new Quaternion(w * tmp, x * tmp, y * tmp, z * tmp);

            }

        }

        public void Inverse() { this = Conjugate() / Dot(this); }
        public Quaternion inversed { get { return Conjugate() / Dot(this); } }

        public override int GetHashCode() {

            return w.GetHashCode() ^ (x.GetHashCode() << 2) ^ (y.GetHashCode() >> 2) ^ (z.GetHashCode() >> 1);

        }
        public override bool Equals(object other) {

            if (!(other is Quaternion)) return false;
            return Equals((Quaternion) other);

        }
        public bool Equals(Quaternion other) {

            return w == other.w && x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "W: {0}, X: {1}, Y: {2}, Z: {3}", w.ToString(), x.ToString(), y.ToString(), z.ToString());

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("ToEuler")]
        internal extern static Vector3 Internal_ToEuler(ref Quaternion quat);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("FromEuler")]
        internal extern static void Internal_FromEuler(ref Vector3 eulerAngles, out Quaternion quat);

    }

}
