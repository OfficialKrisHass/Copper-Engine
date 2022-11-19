using System;

namespace Copper {

    public class Vector2 {


        public Vector2(float x, float y) {

            this.x = x;
            this.y = y;

        }
        public Vector2(float scalar) {

            x = scalar;
            y = scalar;

        }

        public float x;
        public float y;

        public static Vector2 zero      = new Vector2(0.0f);
        public static Vector2 one       = new Vector2(1.0f);
        public static Vector2 minusOne  = new Vector2(-1.0f);

        public static Vector2 operator +(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
        public static Vector2 operator -(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
        public static Vector2 operator *(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
        public static Vector2 operator /(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.x / rhs.x, lhs.y / rhs.y); }

    }

    public struct Vector3 {

        public Vector3(float x, float y, float z) {

            this.x = x;
            this.y = y;
            this.z = z;

        }
        public Vector3(float scalar) {

            x = scalar;
            y = scalar;
            z = scalar;

        }

        public float x;
        public float y;
        public float z;

        public static Vector3 zero = new Vector3(0.0f);
        public static Vector3 one = new Vector3(1.0f);
        public static Vector3 minusOne = new Vector3(-1.0f);

        public static Vector3 operator+(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
        public static Vector3 operator-(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
        public static Vector3 operator*(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
        public static Vector3 operator/(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }

    }

}