using System;
using System.Runtime.InteropServices;
using System.Globalization;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4 {

        // Constructors

        public Vector4(float all) {

            this.x = all;
            this.y = all;
            this.z = all;
            this.w = all;

        }
        public Vector4(float x, float y, float z, float w) {

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

        public Vector4 (Vector3 xyz, float w = 0.0f) {

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

        // Variables

        public float x;
        public float y;
        public float z;
        public float w;

        // Types

        public readonly static Vector4 zero = new Vector4(0.0f);
        public readonly static Vector4 one = new Vector4(1.0f);
        public readonly static Vector4 minusOne = new Vector4(-1.0f);

        // Math functions

        public float Dot(Vector4 other) { return x * other.x + y * other.y + z * other.z + w * other.w; }

        public float lengthSq { get { return x * x + y * y + z * z + w * w; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z + w * w); } }
        
        public Vector4 normalized {

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

        public static Vector4 operator+(Vector4 a, Vector4 b) { return new Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
        public static Vector4 operator-(Vector4 a, Vector4 b) { return new Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
        public static Vector4 operator*(Vector4 a, Vector4 b) { return new Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
        public static Vector4 operator/(Vector4 a, Vector4 b) { return new Vector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

        public static Vector4 operator+(Vector4 a, float b) { return new Vector4(a.x + b, a.y + b, a.z + b, a.w + b); }
        public static Vector4 operator-(Vector4 a, float b) { return new Vector4(a.x - b, a.y - b, a.z - b, a.w - b); }
        public static Vector4 operator*(Vector4 a, float b) { return new Vector4(a.x * b, a.y * b, a.z * b, a.w * b); }
        public static Vector4 operator/(Vector4 a, float b) { return new Vector4(a.x / b, a.y / b, a.z / b, a.w / b); }

        public static Vector4 operator+(float a, Vector4 b) { return b + a; }
        public static Vector4 operator*(float a, Vector4 b) { return b * a; }

        public static Vector4 operator-(Vector4 v) { return new Vector4(-v.x, -v.y, -v.z, -v.w); }

        // Comparison operators

        public static bool operator==(Vector4 a, Vector4 b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
        public static bool operator!=(Vector4 a, Vector4 b) { return !(a == b); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2) ^ (w.GetHashCode() >> 1);

        }
        public override bool Equals(object other) {

            if (!(other is Vector4)) return false;
            return Equals((Vector4) other);

        }
        public bool Equals(Vector4 other) {

            return x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}, W: {3}", x.ToString(), y.ToString(), z.ToString(), w.ToString());

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4I {

        // Constructors

        public Vector4I(int all) {

            this.x = all;
            this.y = all;
            this.z = all;
            this.w = all;

        }
        public Vector4I(int x, int y, int z, int w) {

            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;

        }

        public Vector4I(Vector2I xy, int z = 0, int w = 0) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;
            this.w = w;

        }
        public Vector4I(int x, Vector2I yz, int w = 0) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;
            this.w = w;

        }
        public Vector4I(int x, int y, Vector2I zw) {

            this.x = x;
            this.y = y;
            this.z = zw.x;
            this.w = zw.y;

        }
        public Vector4I(Vector2I xy, Vector2I zw) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = zw.x;
            this.w = zw.y;

        }

        public Vector4I(Vector3I xyz, int w = 0) {

            this.x = xyz.x;
            this.y = xyz.y;
            this.z = xyz.z;
            this.w = w;

        }
        public Vector4I(int x, Vector3I yzw) {

            this.x = x;
            this.y = yzw.x;
            this.z = yzw.y;
            this.w = yzw.z;

        }

        // Variables

        public int x;
        public int y;
        public int z;
        public int w;

        // Types

        public readonly static Vector4I zero = new Vector4I(0);
        public readonly static Vector4I one = new Vector4I(1);
        public readonly static Vector4I minusOne = new Vector4I(-1);

        // Math functions

        public float Dot(Vector4I other) { return x * other.x + y * other.y + z * other.z + w * other.w; }

        public float lengthSq { get { return x * x + y * y + z * z + w * w; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z + w * w); } }
        
        public Vector4I normalized {

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

        public static Vector4I operator+(Vector4I a, Vector4I b) { return new Vector4I(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
        public static Vector4I operator-(Vector4I a, Vector4I b) { return new Vector4I(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
        public static Vector4I operator*(Vector4I a, Vector4I b) { return new Vector4I(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
        public static Vector4I operator/(Vector4I a, Vector4I b) { return new Vector4I(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

        public static Vector4I operator+(Vector4I a, int b) { return new Vector4I(a.x + b, a.y + b, a.z + b, a.w + b); }
        public static Vector4I operator-(Vector4I a, int b) { return new Vector4I(a.x - b, a.y - b, a.z - b, a.w - b); }
        public static Vector4I operator*(Vector4I a, int b) { return new Vector4I(a.x * b, a.y * b, a.z * b, a.w * b); }
        public static Vector4I operator/(Vector4I a, int b) { return new Vector4I(a.x / b, a.y / b, a.z / b, a.w / b); }

        public static Vector4I operator+(int a, Vector4I b) { return b + a; }
        public static Vector4I operator*(int a, Vector4I b) { return b * a; }

        public static Vector4I operator-(Vector4I v) { return new Vector4I(-v.x, -v.y, -v.z, -v.w); }

        // Comparison operators

        public static bool operator==(Vector4I a, Vector4I b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
        public static bool operator!=(Vector4I a, Vector4I b) { return !(a == b); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2) ^ (w.GetHashCode() >> 1);

        }
        public override bool Equals(object other) {

            if (!(other is Vector4I)) return false;
            return Equals((Vector4I) other);

        }
        public bool Equals(Vector4I other) {

            return x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}, W: {3}", x.ToString(), y.ToString(), z.ToString(), w.ToString());

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct UVector4I {

        // Constructors

        public UVector4I(uint all) {

            this.x = all;
            this.y = all;
            this.z = all;
            this.w = all;

        }
        public UVector4I(uint x, uint y, uint z, uint w) {

            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;

        }

        public UVector4I(UVector2I xy, uint z = 0, uint w = 0) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = z;
            this.w = w;

        }
        public UVector4I(uint x, UVector2I yz, uint w = 0) {

            this.x = x;
            this.y = yz.x;
            this.z = yz.y;
            this.w = w;

        }
        public UVector4I(uint x, uint y, UVector2I zw) {

            this.x = x;
            this.y = y;
            this.z = zw.x;
            this.w = zw.y;

        }
        public UVector4I(UVector2I xy, UVector2I zw) {

            this.x = xy.x;
            this.y = xy.y;
            this.z = zw.x;
            this.w = zw.y;

        }

        public UVector4I(UVector3I xyz, uint w = 0) {

            this.x = xyz.x;
            this.y = xyz.y;
            this.z = xyz.z;
            this.w = w;

        }
        public UVector4I(uint x, UVector3I yzw) {

            this.x = x;
            this.y = yzw.x;
            this.z = yzw.y;
            this.w = yzw.z;

        }

        // Variables

        public uint x;
        public uint y;
        public uint z;
        public uint w;

        // Types

        public readonly static UVector4I zero = new UVector4I(0);
        public readonly static UVector4I one = new UVector4I(1);

        // Math functions

        public float Dot(UVector4I other) { return x * other.x + y * other.y + z * other.z + w * other.w; }

        public float lengthSq { get { return x * x + y * y + z * z + w * w; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y + z * z + w * w); } }
        
        public UVector4I normalized {

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

        public static UVector4I operator+(UVector4I a, UVector4I b) { return new UVector4I(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
        public static UVector4I operator-(UVector4I a, UVector4I b) { return new UVector4I(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
        public static UVector4I operator*(UVector4I a, UVector4I b) { return new UVector4I(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
        public static UVector4I operator/(UVector4I a, UVector4I b) { return new UVector4I(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

        public static UVector4I operator+(UVector4I a, uint b) { return new UVector4I(a.x + b, a.y + b, a.z + b, a.w + b); }
        public static UVector4I operator-(UVector4I a, uint b) { return new UVector4I(a.x - b, a.y - b, a.z - b, a.w - b); }
        public static UVector4I operator*(UVector4I a, uint b) { return new UVector4I(a.x * b, a.y * b, a.z * b, a.w * b); }
        public static UVector4I operator/(UVector4I a, uint b) { return new UVector4I(a.x / b, a.y / b, a.z / b, a.w / b); }

        public static UVector4I operator+(uint a, UVector4I b) { return b + a; }
        public static UVector4I operator*(uint a, UVector4I b) { return b * a; }

        // Comparison operators

        public static bool operator==(UVector4I a, UVector4I b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
        public static bool operator!=(UVector4I a, UVector4I b) { return !(a == b); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2) ^ (z.GetHashCode() >> 2) ^ (w.GetHashCode() >> 1);

        }
        public override bool Equals(object other) {

            if (!(other is UVector4I)) return false;
            return Equals((UVector4I) other);

        }
        public bool Equals(UVector4I other) {

            return x == other.x && y == other.y && z == other.z;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}, Z: {2}, W: {3}", x.ToString(), y.ToString(), z.ToString(), w.ToString());

        }

    }

}
