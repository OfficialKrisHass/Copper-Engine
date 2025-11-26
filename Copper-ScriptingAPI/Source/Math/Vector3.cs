using System;
using System.Runtime.InteropServices;
using System.Globalization;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3 {

        // Constructors

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

        // Variables

        public float x;
        public float y;
        public float z;

        // Types

        public readonly static Vector3 zero = new Vector3(0.0f);
        public readonly static Vector3 one = new Vector3(1.0f);
        public readonly static Vector3 minusOne = new Vector3(-1.0f);

        public readonly static Vector3 forward = new Vector3(0.0f, 0.0f, -1.0f);
        public readonly static Vector3 right = new Vector3(1.0f, 0.0f, 0.0f);
        public readonly static Vector3 up = new Vector3(0.0f, 1.0f, 0.0f);

        // Math functions

        public float Dot(Vector3 other) { return x * other.x + y * other.y + z * other.z; }
        public Vector3 Cross(Vector3 other) {

            return new Vector3(y * other.z - z * other.y,
                               z * other.x - x * other.z,
                               x * other.y - y * other.x);

        }

        public float lengthSq { get { return x * x + y * y + z * z; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z); } }
        
        public Vector3 normalized {

            get {

                if (length == 0.0f) return zero;
                return this / length;

            }

        }
        public void Normalize() {

            if (length == 0.0f) return;
            this = this / length;

        }

        // Math operators

        public static Vector3 operator+(Vector3 a, Vector3 b) { return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static Vector3 operator-(Vector3 a, Vector3 b) { return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static Vector3 operator*(Vector3 a, Vector3 b) { return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
        public static Vector3 operator/(Vector3 a, Vector3 b) { return new Vector3(a.x / b.x, a.y / b.y, a.z / b.z); }

        public static Vector3 operator+(Vector3 a, float b) { return new Vector3(a.x + b, a.y + b, a.z + b); }
        public static Vector3 operator-(Vector3 a, float b) { return new Vector3(a.x - b, a.y - b, a.z - b); }
        public static Vector3 operator*(Vector3 a, float b) { return new Vector3(a.x * b, a.y * b, a.z * b); }
        public static Vector3 operator/(Vector3 a, float b) { return new Vector3(a.x / b, a.y / b, a.z / b); }

        public static Vector3 operator+(float a, Vector3 b) { return b + a; }
        public static Vector3 operator*(float a, Vector3 b) { return b * a; }

        public static Vector3 operator-(Vector3 v) { return new Vector3(-v.x, -v.y, -v.z); }

        // Comparison operators

        public static bool operator==(Vector3 a, Vector3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
        public static bool operator!=(Vector3 a, Vector3 b) { return !(a == b); }

        // Conversion operators

        public static implicit operator Vector3(Vector4 v) { return new Vector3(v.x, v.y, v.z); }
        public static implicit operator Vector4(Vector3 v) { return new Vector4(v.x, v.y, v.z, 0.0f); }

        // Overrides

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

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}", x.ToString(), y.ToString(), z.ToString());

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3I {

        // Constructors

        public Vector3I(int all) {

            this.x = all;
            this.y = all;
            this.z = all;

        }
        public Vector3I(int x, int y, int z) {

            this.x = x;
            this.y = y;
            this.z = z;

        }

        public Vector3I(Vector2I xy, int z = 0) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;

        }
        public Vector3I(int x, Vector2I yz) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;

        }

        // Variables

        public int x;
        public int y;
        public int z;

        // Types

        public readonly static Vector3I zero = new Vector3I(0);
        public readonly static Vector3I one = new Vector3I(1);
        public readonly static Vector3I minusOne = new Vector3I(-1);

        public readonly static Vector3I forward = new Vector3I(0, 0, -1);
        public readonly static Vector3I right = new Vector3I(1, 0, 0);
        public readonly static Vector3I up = new Vector3I(0, 1, 0);

        // Math functions

        public float Dot(Vector3I other) { return x * other.x + y * other.y + z * other.z; }
        public Vector3I Cross(Vector3I other) {

            return new Vector3I(y * other.z - z * other.y,
                                z * other.x - x * other.z,
                                x * other.y - y * other.x);

        }

        public float lengthSq { get { return x * x + y * y + z * z; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z); } }

        public Vector3I normalized {

            get {

                if (length == 0.0f) return zero;
                return this / (int) length;

            }

        }
        public void Normalize() {

            if (length == 0.0f) return;
            this = this / (int) length;

        }

        // Math operators

        public static Vector3I operator+(Vector3I a, Vector3I b) { return new Vector3I(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static Vector3I operator-(Vector3I a, Vector3I b) { return new Vector3I(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static Vector3I operator*(Vector3I a, Vector3I b) { return new Vector3I(a.x * b.x, a.y * b.y, a.z * b.z); }
        public static Vector3I operator/(Vector3I a, Vector3I b) { return new Vector3I(a.x / b.x, a.y / b.y, a.z / b.z); }

        public static Vector3I operator+(Vector3I a, int b) { return new Vector3I(a.x + b, a.y + b, a.z + b); }
        public static Vector3I operator-(Vector3I a, int b) { return new Vector3I(a.x - b, a.y - b, a.z - b); }
        public static Vector3I operator*(Vector3I a, int b) { return new Vector3I(a.x * b, a.y * b, a.z * b); }
        public static Vector3I operator/(Vector3I a, int b) { return new Vector3I(a.x / b, a.y / b, a.z / b); }

        public static Vector3I operator+(int a, Vector3I b) { return b + a; }
        public static Vector3I operator*(int a, Vector3I b) { return b * a; }

        public static Vector3I operator-(Vector3I v) { return new Vector3I(-v.x, -v.y, -v.z); }

        // Comparison operators

        public static bool operator==(Vector3I a, Vector3I b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
        public static bool operator!=(Vector3I a, Vector3I b) { return !(a == b); }

        // Conversion operators

        public static implicit operator Vector3I(Vector4I v) { return new Vector3I(v.x, v.y, v.z); }
        public static implicit operator Vector4I(Vector3I v) { return new Vector4I(v.x, v.y, v.z, 0); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2);

        }
        public override bool Equals(object other) {

            if (!(other is Vector3I)) return false;
            return Equals((Vector3I) other);

        }
        public bool Equals(Vector3I other) {

            return x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}", x.ToString(), y.ToString(), z.ToString());

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct UVector3I {

        // Constructors

        public UVector3I(uint all) {

            this.x = all;
            this.y = all;
            this.z = all;

        }
        public UVector3I(uint x, uint y, uint z) {

            this.x = x;
            this.y = y;
            this.z = z;

        }

        public UVector3I(UVector3I xy, uint z = 0) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;

        }
        public UVector3I(uint x, UVector3I yz) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;

        }

        // Variables

        public uint x;
        public uint y;
        public uint z;

        // Types

        public readonly static UVector3I zero = new UVector3I(0);
        public readonly static UVector3I one = new UVector3I(1);

        public readonly static UVector3I right = new UVector3I(1, 0, 0);
        public readonly static UVector3I up = new UVector3I(0, 1, 0);

        // Math functions

        public float Dot(UVector3I other) { return x * other.x + y * other.y + z * other.z; }
        public UVector3I Cross(UVector3I other) {

            return new UVector3I(y * other.z - z * other.y,
                                 z * other.x - x * other.z,
                                 x * other.y - y * other.x);

        }

        public float lengthSq { get { return x * x + y * y + z * z; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z); } }
        
        public UVector3I normalized {

            get {

                if (length == 0.0f) return zero;
                return this / (uint) length;

            }

        }
        public void Normalize() {

            if (length == 0.0f) return;
            this = this / (uint) length;

        }

        // Math operators

        public static UVector3I operator+(UVector3I a, UVector3I b) { return new UVector3I(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static UVector3I operator-(UVector3I a, UVector3I b) { return new UVector3I(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static UVector3I operator*(UVector3I a, UVector3I b) { return new UVector3I(a.x * b.x, a.y * b.y, a.z * b.z); }
        public static UVector3I operator/(UVector3I a, UVector3I b) { return new UVector3I(a.x / b.x, a.y / b.y, a.z / b.z); }

        public static UVector3I operator+(UVector3I a, uint b) { return new UVector3I(a.x + b, a.y + b, a.z + b); }
        public static UVector3I operator-(UVector3I a, uint b) { return new UVector3I(a.x - b, a.y - b, a.z - b); }
        public static UVector3I operator*(UVector3I a, uint b) { return new UVector3I(a.x * b, a.y * b, a.z * b); }
        public static UVector3I operator/(UVector3I a, uint b) { return new UVector3I(a.x / b, a.y / b, a.z / b); }

        public static UVector3I operator+(uint a, UVector3I b) { return b + a; }
        public static UVector3I operator*(uint a, UVector3I b) { return b * a; }

        // Comparison operators

        public static bool operator==(UVector3I a, UVector3I b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
        public static bool operator!=(UVector3I a, UVector3I b) { return !(a == b); }

        // Conversion operators

        public static implicit operator UVector3I(UVector4I v) { return new UVector3I(v.x, v.y, v.z); }
        public static implicit operator UVector4I(UVector3I v) { return new UVector4I(v.x, v.y, v.z, 0); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2);

        }
        public override bool Equals(object other) {

            if (!(other is UVector3I)) return false;
            return Equals((UVector3I) other);

        }
        public bool Equals(UVector3I other) {

            return x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}", x.ToString(), y.ToString(), z.ToString());

        }

    }

}
