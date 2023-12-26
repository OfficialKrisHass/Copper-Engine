using System;

namespace Copper {

    public struct Vector4 {

        public Vector4(float all) {

            this.x = all;
            this.y = all;
            this.z = all;
            this.w = all;

        }
        public Vector4(float x, float y, float z, float w = 0.0f) {

            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;

        }

        public Vector4(Vector2 xy, float z = 0.0f, float w = 0.0f) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;
            this.w = w;

        }
        public Vector4(float x, Vector2 yz, float w = 0.0f) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;
            this.w = w;

        }
        public Vector4(float x, float y, Vector2 zw) {

            this.x = x;
            this.y = y;
            this.z = zw.x;
            this.w = zw.y;

        }
        public Vector4(Vector2 xy, Vector2 zw) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = zw.x;
            this.w = zw.y;

        }

        public Vector4(Vector3 xyz, float w = 0.0f) {

            this.x = xyz.x;
            this.y = xyz.y;
            this.z = xyz.z;
            this.w = w;

        }
        public Vector4(float x, Vector3 yzw) {

            this.x = x;
            this.y = yzw.x;
            this.z = yzw.y;
            this.w = yzw.z;

        }

        public float x;
        public float y;
        public float z;
        public float w;

        public static readonly Vector4 zero = new Vector4(0.0f);
        public static readonly Vector4 one = new Vector4(1.0f);
        public static readonly Vector4 minusOne = new Vector4(-1.0f);

        public float Dot(Vector4 other) { return x * other.x + y * other.y + z * other.z + w * other.w; }

        public void Normalize() { this = normalized; }

        public float lengthSq { get { return x * x + y * y + z * z + w * w; } }
        public float length { get { return (float)Math.Sqrt(x * x + y * y + z * z + w * w); } }

        public Vector4 normalized { get { return this /= length; } }

        public static Vector4 operator +(Vector4 lhs, Vector4 rhs) { return new Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
        public static Vector4 operator -(Vector4 lhs, Vector4 rhs) { return new Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
        public static Vector4 operator *(Vector4 lhs, Vector4 rhs) { return new Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
        public static Vector4 operator /(Vector4 lhs, Vector4 rhs) { return new Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }
        public static Vector4 operator %(Vector4 lhs, Vector4 rhs) { return new Vector4(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z, lhs.w % rhs.w); }

        public static Vector4 operator +(Vector4 lhs, float rhs) { return new Vector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
        public static Vector4 operator -(Vector4 lhs, float rhs) { return new Vector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
        public static Vector4 operator *(Vector4 lhs, float rhs) { return new Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
        public static Vector4 operator /(Vector4 lhs, float rhs) { return new Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }
        public static Vector4 operator %(Vector4 lhs, float rhs) { return new Vector4(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs, lhs.w % rhs); }

        public static bool operator ==(Vector4 lhs, Vector4 rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }
        public static bool operator !=(Vector4 lhs, Vector4 rhs) { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w; }

        public static Vector4 operator ++(Vector4 lhs) { return new Vector4(lhs.x + 1, lhs.y + 1, lhs.z + 1, lhs.w + 1); }
        public static Vector4 operator --(Vector4 lhs) { return new Vector4(lhs.x - 1, lhs.y - 1, lhs.z - 1, lhs.w - 1); }

        public static Vector4 operator +(Vector4 lhs) { return lhs; }
        public static Vector4 operator -(Vector4 lhs) { return new Vector4(-lhs.x, -lhs.y, -lhs.z, -lhs.w); }

        public override bool Equals(object obj) { return this == (Vector4) obj; }
        public override int GetHashCode() { return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode() ^ w.GetHashCode(); }

        public override string ToString() { return "X: {" + x + "} Y: {" + y + "} Z: {" + z + "} W: {" + w + "}"; }

    };

}