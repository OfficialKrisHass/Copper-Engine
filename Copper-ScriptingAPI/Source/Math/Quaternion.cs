using System;
using System.Runtime.CompilerServices;

namespace Copper {

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

        public float w;
        public float x;
        public float y;
        public float z;

        public static readonly Quaternion identity = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

        public float Dot(Quaternion other) { return (x * other.x + y * other.y) + (z * other.z + w * other.w); }
        public Quaternion Cross(Quaternion other) {

            return new Quaternion(w * other.w - x * other.x - y * other.y - z * other.z,
                                  w * other.x + x * other.w + y * other.z - z * other.y,
                                  w * other.y + y * other.w + z * other.x - x * other.z,
                                  w * other.z + z * other.w + x * other.y - y * other.x);

        }

        public void Normaize() { this = normalized; }

        public float length {  get { return (float) Math.Sqrt(Dot(this)); } }

        public Quaternion conjugate { get { return new Quaternion(w, -x, -y, -z); } }
        public Quaternion normalized { get {

                float length = this.length;
                if (length <= 0.0f) return Quaternion.identity;

                float tmp = 1.0f / length;
                return new Quaternion(w * tmp, x * tmp, y * tmp, z * tmp);

            } }
        public Quaternion inverse { get { return conjugate / Dot(this); } }

        public Vector3 eulerAngles {

            get { return Internal_ToEuler(this); }
            set { this = Internal_FromEuler(value); }

        }

        public static Quaternion Euler(Vector3 euler) { return Internal_FromEuler(euler); }
        public static Quaternion Euler(float x, float y, float z) { return Internal_FromEuler(new Vector3(x, y, z)); }

        public static Quaternion operator+(Quaternion lhs, Quaternion rhs) { return new Quaternion(lhs.w + rhs.w, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
        public static Quaternion operator-(Quaternion lhs, Quaternion rhs) { return new Quaternion(lhs.w - rhs.w, lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
        public static Quaternion operator*(Quaternion lhs, Quaternion rhs) {

            return new Quaternion(lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
                                  lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                                  lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                                  lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x);

        }

        public static Quaternion operator*(Quaternion lhs, float rhs) { return new Quaternion(lhs.w * rhs, lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
        public static Quaternion operator/(Quaternion lhs, float rhs) { return new Quaternion(lhs.w / rhs, lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }

        public static Vector3 operator*(Quaternion lhs, Vector3 rhs) {

            Vector3 vec = new Vector3(lhs.x, lhs.y, lhs.z);
            Vector3 uv = vec.Cross(rhs);
            Vector3 uuv = vec.Cross(uv);

            return rhs + ((uv * lhs.w) + uuv) * 2.0f;

        }
        public static Vector4 operator*(Quaternion lhs, Vector4 rhs) { return new Vector4(lhs * new Vector3(rhs), rhs.w); }

        public static bool operator==(Quaternion lhs, Quaternion rhs) { return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
        public static bool operator!=(Quaternion lhs, Quaternion rhs) { return lhs.w != rhs.w || lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; }

        public static Quaternion operator+(Quaternion lhs) { return lhs; }
        public static Quaternion operator-(Quaternion lhs) { return new Quaternion(-lhs.w, -lhs.x, -lhs.y, -lhs.z); }

        public override bool Equals(object obj) { return this == (Quaternion) obj; }
        public override int GetHashCode() { return w.GetHashCode() ^ x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode(); }

        public override string ToString() { return "W: {" + w + "} X: {" + x + "} Y: {" + y + "} Z: {" + z + "}"; }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Vector3 Internal_ToEuler(Quaternion quaternion);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Quaternion Internal_FromEuler(Vector3 quaternion);

    }

}