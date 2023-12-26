using System;

namespace Copper {

    public struct Vector3 {

        public Vector3(float all) {

            this.x = all;
            this.y = all;
            this.z = all;

        }
        public Vector3(float x, float y, float z = 0.0f) {

            this.x = x;
            this.y = y;
            this.z = z;

        }

        public Vector3(Vector2 xy, float z = 0.0f) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;

        }
        public Vector3(float x, Vector2 yz) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;

        }

        public Vector3(Vector4 vec) {

            this.x = vec.x;
            this.y = vec.y;
            this.z = vec.z;

        }

        public float x;
        public float y;
        public float z;

        public static readonly Vector3 zero = new Vector3(0.0f);
        public static readonly Vector3 one = new Vector3(1.0f);
        public static readonly Vector3 minusOne = new Vector3(-1.0f);

        public float Dot(Vector3 other) { return x * other.x + y * other.y + z * other.z; }
        public Vector3 Cross(Vector3 other) {

            return new Vector3(y * other.z - z * other.y,
                               z * other.x - x * other.z,
                               x * other.y - y * other.x);

        }

        public void Normalize() { this = normalized; }

        public float lengthSq { get { return x * x + y * y + z * z; } }
        public float length { get { return (float)Math.Sqrt(x * x + y * y + z * z); } }

        public Vector3 normalized { get { return this /= length; } }

        public static Vector3 operator +(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
        public static Vector3 operator -(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
        public static Vector3 operator *(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
        public static Vector3 operator /(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
        public static Vector3 operator %(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z); }
                      
        public static Vector3 operator +(Vector3 lhs, float rhs) { return new Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
        public static Vector3 operator -(Vector3 lhs, float rhs) { return new Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
        public static Vector3 operator *(Vector3 lhs, float rhs) { return new Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
        public static Vector3 operator /(Vector3 lhs, float rhs) { return new Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
        public static Vector3 operator %(Vector3 lhs, float rhs) { return new Vector3(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs); }

        public static bool operator ==(Vector3 lhs, Vector3 rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
        public static bool operator !=(Vector3 lhs, Vector3 rhs) { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; }

        public static Vector3 operator ++(Vector3 lhs) { return new Vector3(lhs.x + 1, lhs.y + 1, lhs.z + 1); }
        public static Vector3 operator --(Vector3 lhs) { return new Vector3(lhs.x - 1, lhs.y - 1, lhs.z - 1); }

        public static Vector3 operator +(Vector3 lhs) { return lhs; }
        public static Vector3 operator -(Vector3 lhs) { return new Vector3(-lhs.x, -lhs.y, -lhs.z); }

        public override bool Equals(object obj) { return this == (Vector3) obj; }
        public override int GetHashCode() { return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode(); }

        public override string ToString() { return "X: {" + x + "} Y: {" + y + "} Z: {" + z + "}"; }

    };

}