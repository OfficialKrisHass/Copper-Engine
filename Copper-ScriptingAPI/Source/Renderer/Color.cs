using System;
using System.Runtime.InteropServices;
using System.Globalization;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public struct Color {

        public Color(float all, float a = 1.0f) {

            this.r = all;
            this.g = all;
            this.b = all;
            this.a = a;

        }
        public Color(float r, float g, float b, float a = 1.0f) {

            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;

        }

        public Color(Vector3 v, float a = 1.0f) {

            this.r = v.x;
            this.g = v.y;
            this.b = v.z;
            this.a = a;

        }

        float r;
        float g;
        float b;
        float a;

        public readonly static Color red = new Color(1.0f, 0.0f, 0.0f);
        public readonly static Color green = new Color(0.0f, 1.0f, 0.0f);
        public readonly static Color blue = new Color(0.0f, 0.0f, 1.0f);

        public readonly static Color white = new Color(1.0f);
        public readonly static Color black = new Color(0.0f);

        public readonly static Color transparent = new Color(1.0f, 0.0f);

        public static bool operator==(Color a, Color b) { return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; }
        public static bool operator!=(Color a, Color b) { return !(a == b); }

        public static implicit operator Vector3(Color c) { return new Vector3(c.r, c.g, c.b); }

        public override int GetHashCode() {

            return r.GetHashCode() ^ (g.GetHashCode() << 2) ^ (b.GetHashCode() >> 2) ^ (a.GetHashCode() >> 1);

        }
        public override bool Equals(object other) {

            if (!(other is Color)) return false;
            return Equals((Color) other);

        }
        public bool Equals(Color other) {

            return r == other.r && b == other.b && g == other.g && a == other.a;

        }

        public override string ToString() {

            return String.Format(CultureInfo.InvariantCulture.NumberFormat, "R: {0}, G: {1}, B: {2}, A: {3}", r.ToString(), g.ToString(), b.ToString(), a.ToString());

        }

    }

}
