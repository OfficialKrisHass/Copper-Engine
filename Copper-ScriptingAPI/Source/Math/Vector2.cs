using System;
using System.Runtime.InteropServices;
using System.Globalization;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 {

        // Constructors

        public Vector2(float all) {

            this.x = all;
            this.y = all;

        }
        public Vector2(float x, float y) {

            this.x = x;
            this.y = y;

        }

        // Variables 

        public float x;
        public float y;

        public readonly static Vector2 zero = new Vector2(0.0f);
        public readonly static Vector2 one = new Vector2(1.0f);
        public readonly static Vector2 minusOne = new Vector2(-1.0f);

        // Math functions

        public float Dot(Vector2 other) { return x * other.x + y * other.y; }

        public float lengthSq { get { return x * x + y * y; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y); } }

        public Vector2 normalized {

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

        public static Vector2 operator+(Vector2 a, Vector2 b) { return new Vector2(a.x + b.x, a.y + b.y); }
        public static Vector2 operator-(Vector2 a, Vector2 b) { return new Vector2(a.x - b.x, a.y - b.y); }
        public static Vector2 operator*(Vector2 a, Vector2 b) { return new Vector2(a.x * b.x, a.y * b.y); }
        public static Vector2 operator/(Vector2 a, Vector2 b) { return new Vector2(a.x / b.x, a.y / b.y); }

        public static Vector2 operator+(Vector2 a, float b) { return new Vector2(a.x + b, a.y + b); }
        public static Vector2 operator-(Vector2 a, float b) { return new Vector2(a.x - b, a.y - b); }
        public static Vector2 operator*(Vector2 a, float b) { return new Vector2(a.x * b, a.x * b); }
        public static Vector2 operator/(Vector2 a, float b) { return new Vector2(a.x / b, a.y / b); }

        public static Vector2 operator+(float a, Vector2 b) { return b + a; }
        public static Vector2 operator*(float a, Vector2 b) { return b * a; }

        public static Vector2 operator-(Vector2 v) { return new Vector2(-v.x, -v.y); }

        // Comparison operators

        public static bool operator==(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }
        public static bool operator!=(Vector2 a, Vector2 b) { return !(a == b); }

        // Conversion operators

        public static implicit operator Vector2(Vector3 v) { return new Vector2(v.x, v.y); }
        public static implicit operator Vector3(Vector2 v) { return new Vector3(v.x, v.y, 0.0f); }

        // Overrides

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

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}", x.ToString(), y.ToString());

        }

    } 

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2I {

        // Constructors

        public Vector2I(int all) {

            this.x = all;
            this.y = all;

        }
        public Vector2I(int x, int y) {

            this.x = x;
            this.y = y;

        }

        // Variables 

        public int x;
        public int y;

        public readonly static Vector2I zero = new Vector2I(0);
        public readonly static Vector2I one = new Vector2I(1);
        public readonly static Vector2I minusOne = new Vector2I(-1);

        // Math functions

        public float Dot(Vector2I other) { return x * other.x + y * other.y; }

        public float lengthSq { get { return x * x + y * y; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y); } }

        public Vector2I normalized {

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

        public static Vector2I operator+(Vector2I a, Vector2I b) { return new Vector2I(a.x + b.x, a.y + b.y); }
        public static Vector2I operator-(Vector2I a, Vector2I b) { return new Vector2I(a.x - b.x, a.y - b.y); }
        public static Vector2I operator*(Vector2I a, Vector2I b) { return new Vector2I(a.x * b.x, a.y * b.y); }
        public static Vector2I operator/(Vector2I a, Vector2I b) { return new Vector2I(a.x / b.x, a.y / b.y); }

        public static Vector2I operator+(Vector2I a, int b) { return new Vector2I(a.x + b, a.y + b); }
        public static Vector2I operator-(Vector2I a, int b) { return new Vector2I(a.x - b, a.y - b); }
        public static Vector2I operator*(Vector2I a, int b) { return new Vector2I(a.x * b, a.x * b); }
        public static Vector2I operator/(Vector2I a, int b) { return new Vector2I(a.x / b, a.y / b); }

        public static Vector2I operator+(int a, Vector2I b) { return b + a; }
        public static Vector2I operator*(int a, Vector2I b) { return b * a; }

        public static Vector2I operator-(Vector2I v) { return new Vector2I(-v.x, -v.y); }

        // Comparison operators

        public static bool operator==(Vector2I a, Vector2I b) { return a.x == b.x && a.y == b.y; }
        public static bool operator!=(Vector2I a, Vector2I b) { return !(a == b); }

        // Conversion operators

        public static implicit operator Vector2I(Vector3I v) { return new Vector2I(v.x, v.y); }
        public static implicit operator Vector3I(Vector2I v) { return new Vector3I(v.x, v.y, 0); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2);

        }
        public override bool Equals(object other) {

            if(!(other is Vector2I)) return false;
            return Equals((Vector2I) other);

        }
        public bool Equals(Vector2I other) {

            return x == other.x && y == other.y;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}", x.ToString(), y.ToString());

        }

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct UVector2I {

        // Constructors

        public UVector2I(uint all) {

            this.x = all;
            this.y = all;

        }
        public UVector2I(uint x, uint y) {

            this.x = x;
            this.y = y;

        }

        // Variables 

        public uint x;
        public uint y;

        public readonly static UVector2I zero = new UVector2I(0);
        public readonly static UVector2I one = new UVector2I(1);

        // Math functions

        public float Dot(UVector2I other) { return x * other.x + y * other.y; }

        public float lengthSq { get { return x * x + y * y; } }
        public float length { get { return (float) Math.Sqrt(x * x + y * y); } }

        public UVector2I normalized {

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

        public static UVector2I operator+(UVector2I a, UVector2I b) { return new UVector2I(a.x + b.x, a.y + b.y); }
        public static UVector2I operator-(UVector2I a, UVector2I b) { return new UVector2I(a.x - b.x, a.y - b.y); }
        public static UVector2I operator*(UVector2I a, UVector2I b) { return new UVector2I(a.x * b.x, a.y * b.y); }
        public static UVector2I operator/(UVector2I a, UVector2I b) { return new UVector2I(a.x / b.x, a.y / b.y); }

        public static UVector2I operator+(UVector2I a, uint b) { return new UVector2I(a.x + b, a.y + b); }
        public static UVector2I operator-(UVector2I a, uint b) { return new UVector2I(a.x - b, a.y - b); }
        public static UVector2I operator*(UVector2I a, uint b) { return new UVector2I(a.x * b, a.x * b); }
        public static UVector2I operator/(UVector2I a, uint b) { return new UVector2I(a.x / b, a.y / b); }

        public static UVector2I operator+(uint a, UVector2I b) { return b + a; }
        public static UVector2I operator*(uint a, UVector2I b) { return b * a; }

        // Comparison operators

        public static bool operator==(UVector2I a, UVector2I b) { return a.x == b.x && a.y == b.y; }
        public static bool operator!=(UVector2I a, UVector2I b) { return !(a == b); }

        // Conversion operators

        public static implicit operator UVector2I(UVector3I v) { return new UVector2I(v.x, v.y); }
        public static implicit operator UVector3I(UVector2I v) { return new UVector3I(v.x, v.y, 0); }

        // Overrides

        public override int GetHashCode() {

            return x.GetHashCode() ^ (y.GetHashCode() << 2);

        }
        public override bool Equals(object other) {

            if(!(other is UVector2I)) return false;
            return Equals((UVector2I) other);

        }
        public bool Equals(UVector2I other) {

            return x == other.x && y == other.y;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "X: {0}, Y: {1}", x.ToString(), y.ToString());

        }

    }

}
