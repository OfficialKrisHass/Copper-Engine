using System;
using System.Runtime.InteropServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 {

        public Vector2(float all) {

            this.x = all;
            this.y = all;

        }
        public Vector2(float x, float y) {

            this.x = x;
            this.y = y;

        }

        public float x;
        public float y;

        public readonly static Vector2 zero = new Vector2(0.0f);
        public readonly static Vector2 one = new Vector2(1.0f);
        public readonly static Vector2 minusOne = new Vector2(-1.0f);

        public static Vector2 operator+(Vector2 a, Vector2 b) { return new Vector2(a.x + b.x, a.y + b.y); }
        public static Vector2 operator-(Vector2 a, Vector2 b) { return new Vector2(a.x - b.x, a.y - b.y); }
        public static Vector2 operator*(Vector2 a, Vector2 b) { return new Vector2(a.x * b.x, a.y * b.y); }
        public static Vector2 operator/(Vector2 a, Vector2 b) { return new Vector2(a.x / b.x, a.y / b.y); }

        public static Vector2 operator+(Vector2 a, float b) { return new Vector2(a.x + b, a.y + b); }
        public static Vector2 operator+(float a, Vector2 b) { return new Vector2(b.x + a, b.y + a); }
        public static Vector2 operator-(Vector2 a, float b) { return new Vector2(a.x - b, a.y - b); }
        public static Vector2 operator*(Vector2 a, float b) { return new Vector2(a.x * b, a.x * b); }
        public static Vector2 operator*(float a, Vector2 b) { return new Vector2(b.x * a, b.y * a); }
        public static Vector2 operator/(Vector2 a, float b) { return new Vector2(a.x / b, a.y / b); }

        public static Vector2 operator-(Vector2 v) { return new Vector2(-v.x, -v.y); }

        public static bool operator==(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }
        public static bool operator!=(Vector2 a, Vector2 b) { return !(a == b); }

        public static implicit operator Vector2(Vector3 v) { return new Vector2(v.x, v.y); }
        public static implicit operator Vector3(Vector2 v) { return new Vector3(v.x, v.y, 0.0f); }

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2);

        }
        public override bool Equals(object other) {

            if(!(other is Vector2)) return false;
            return Equals((Vector2) other);

        }
        public bool Equals(Vector2 other) {

            return x == other.x && y == other.y;

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3 {

        public Vector3(float all) {

            this.x = all;
            this.y = all;
            this.z = all;

        }
        public Vector3(float x, float y, float z) {

            this.x = x;
            this.y = y;
            this.z = z;

        }

        public float x;
        public float y;
        public float z;

        public readonly static Vector3 zero = new Vector3(0.0f);
        public readonly static Vector3 one = new Vector3(1.0f);
        public readonly static Vector3 minusOne = new Vector3(-1.0f);

        public readonly static Vector3 forward = new Vector3(0.0f, 0.0f, -1.0f);
        public readonly static Vector3 right = new Vector3(1.0f, 0.0f, 0.0f);
        public readonly static Vector3 up = new Vector3(0.0f, 1.0f, 0.0f);

        public static Vector3 operator+(Vector3 a, Vector3 b) { return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static Vector3 operator-(Vector3 a, Vector3 b) { return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static Vector3 operator*(Vector3 a, Vector3 b) { return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
        public static Vector3 operator/(Vector3 a, Vector3 b) { return new Vector3(a.x / b.x, a.y / b.y, a.z / b.z); }

        public static Vector3 operator+(Vector3 a, float b) { return new Vector3(a.x + b, a.y + b, a.z + b); }
        public static Vector3 operator+(float a, Vector3 b) { return new Vector3(b.x + a, b.y + a, b.z + a); }
        public static Vector3 operator-(Vector3 a, float b) { return new Vector3(a.x - b, a.y - b, a.z - b); }
        public static Vector3 operator*(Vector3 a, float b) { return new Vector3(a.x * b, a.y * b, a.z * b); }
        public static Vector3 operator*(float a, Vector3 b) { return new Vector3(b.x * a, b.y * a, b.z * a); }
        public static Vector3 operator/(Vector3 a, float b) { return new Vector3(a.x / b, a.y / b, a.z / b); }

        public static Vector3 operator-(Vector3 v) { return new Vector3(-v.x, -v.y, -v.z); }

        public static bool operator==(Vector3 a, Vector3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
        public static bool operator!=(Vector3 a, Vector3 b) { return !(a == b); }

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2);

        }
        public override bool Equals(object other) {

            if (!(other is Vector3)) return false;
            return Equals((Vector3) other);

        }
        public bool Equals(Vector3 other) {

            return x == other.x && y == other.y && z == other.z;

        }

    }

}
