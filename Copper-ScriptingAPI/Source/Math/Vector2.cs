using System;

namespace Copper {

    public struct Vector2 {

        public Vector2(float all) {

            this.x = all;
            this.y = all;

        }
        public Vector2(float x, float y) {

            this.x = x;
            this.y = y;

        }

        public Vector2(Vector3 vec) {

            this.x = vec.x;
            this.y = vec.y;

        }
        public Vector2(Vector4 vec) {

            this.x = vec.x;
            this.y = vec.y;

        }

        public float x;
        public float y;

        public static readonly Vector2 zero = new Vector2(0.0f);
        public static readonly Vector2 one = new Vector2(1.0f);
        public static readonly Vector2 minusOne = new Vector2(-1.0f);

        public float Dot(Vector2 other) { return x * other.x + y * other.y; }

        public void Normalize() { this = normalized; }

        public float lengthSq { get { return x * x + y * y; } }
        public float length { get { return (float)Math.Sqrt(x * x + y * y); } }

        public Vector2 normalized { get { return this /= length; } }

        public static Vector2 operator +(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
        public static Vector2 operator -(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
        public static Vector2 operator *(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
        public static Vector2 operator /(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x / rhs.x, lhs.y / rhs.y); }
        public static Vector2 operator %(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x % rhs.x, lhs.y % rhs.y); }

        public static Vector2 operator +(Vector2 lhs, float rhs) { return new Vector2(lhs.x + rhs, lhs.y + rhs); }
        public static Vector2 operator -(Vector2 lhs, float rhs) { return new Vector2(lhs.x - rhs, lhs.y - rhs); }
        public static Vector2 operator *(Vector2 lhs, float rhs) { return new Vector2(lhs.x * rhs, lhs.y * rhs); }
        public static Vector2 operator /(Vector2 lhs, float rhs) { return new Vector2(lhs.x / rhs, lhs.y / rhs); }
        public static Vector2 operator %(Vector2 lhs, float rhs) { return new Vector2(lhs.x % rhs, lhs.y % rhs); }

        public static bool operator==(Vector2 lhs, Vector2 rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
        public static bool operator!=(Vector2 lhs, Vector2 rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }

        public static Vector2 operator++(Vector2 lhs) { return new Vector2(lhs.x + 1, lhs.y + 1); }
        public static Vector2 operator--(Vector2 lhs) { return new Vector2(lhs.x - 1, lhs.y - 1); }

        public static Vector2 operator+(Vector2 lhs) { return lhs; }
        public static Vector2 operator-(Vector2 lhs) { return new Vector2(-lhs.x, -lhs.y); }

        public override bool Equals(object obj) { return this == (Vector2)obj; }
        public override int GetHashCode() { return x.GetHashCode() ^ y.GetHashCode(); }

        public override string ToString() { return "X: {" + x + "} Y: {" + y + "}"; }

    };

}