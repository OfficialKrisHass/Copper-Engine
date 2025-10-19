using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Core/UUID.cs.h")]
    public struct UUID : IEquatable<UUID> {

        // Constructors

        public UUID(ulong x, ulong y) {

            m_data = new byte[16];
            Internal_Constructor(m_data, x, y);

        }
        public UUID(string prettyString) {

            m_data = new byte[16];
            Internal_SetString(m_data, prettyString);

        }

        private byte[] m_data;

        public static readonly UUID invalid = new UUID(0, 0);

        // Generation

        // little hack since we can't have parameterless constructors, plus native side the default constructor
        // constructs an invalid UUID. That is why this variable is unused.
        private UUID(bool unused) {

            _ = unused;

            m_data = new byte[16];
            Internal_GenerateUUID(m_data);

        }

        public static UUID Generate() { return new UUID(false); }
        public void Regenerate() {

            Internal_GenerateUUID(m_data);

        }

        // String

        public static UUID FromString(string prettyString) { return new UUID(prettyString); }
        public void SetString(string prettyString) {

            Internal_SetString(m_data, prettyString);

        }

        public override string ToString() {

            return Internal_ToString(m_data);

        }

        // Operators

        public static bool operator==(UUID a, UUID b) {

            return Internal_Equals(a.m_data, b.m_data);

        }
        public static bool operator!=(UUID a, UUID b) { return !(a == b); }

        public static bool operator<(UUID a, UUID b) {

            return Internal_LessThan(a.m_data, b.m_data);

        }
        public static bool operator>(UUID a, UUID b) { return b < a; }
        public static bool operator<=(UUID a, UUID b) { return !(a > b); }
        public static bool operator>=(UUID a, UUID b) { return !(a < b); }

        // Overrides

        public override int GetHashCode() {

            return Internal_Hash(m_data);

        }

        public override bool Equals(object other) {

            if (!(other is UUID)) return false;
            return Equals((UUID) other);

        }
        public bool Equals(UUID other) {

            return Internal_Equals(m_data, other.m_data);

        }

        // Internal calls

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("Constructor")]
        internal extern static void Internal_Constructor(byte[] data, ulong x, ulong y);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GenerateUUID")]
        internal extern static void Internal_GenerateUUID(byte[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("SetString")]
        internal extern static void Internal_SetString(byte[] data, string prettyString);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("ToString")]
        internal extern static string Internal_ToString(byte[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("Equals")]
        internal extern static bool Internal_Equals(byte[] a, byte[] b);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("LessThan")]
        internal extern static bool Internal_LessThan(byte[] a, byte[] b);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("Hash")]
        internal extern static int Internal_Hash(byte[] data);

    }

}
