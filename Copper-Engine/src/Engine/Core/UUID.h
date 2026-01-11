#pragma once

#include "Engine/Core/Core.h"

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir 

#define UUID_FUNC_IMPL(func, ...) CUP_FUNCTION(); EnsureInitialized(); CU_ASSERT(&Get ## func ## Impl() != nullptr, "UUID:: Get" #func "Impl() was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type)); Get ## func ## Impl()(__VA_ARGS__)
#define UUID_GET_IMPL(signature, name) inline static std::function<signature>& Get ## name ## Impl() { static std::function<signature> name ## _impl = nullptr; return name ## _impl; }

namespace Copper {

    // A wrapper around a 16 byte byte array containing a RFC 4122 UUID.
    // Contains SIMD code that gets enabled and disabled during startup or manually using UUID::SetType.
    struct UUID {

    public:
        // Determines which functions to use. Assigned automatically at startup, or manually using UUID::SetType().
        enum class Type : uint8 {

            None = 0,

            Scalar,     // No SIMD instructions
            SIMD_SSE4,  // SSE4 SIMD instructions
            SIMD_AVX2   // SSE4 + AVX2 SIMD instructions

        };

        // Creates a nil UUID (all zeroes)
        UUID() : UUID(0, 0) {}

        inline UUID(const UUID& other) { UUID_FUNC_IMPL(Set, m_data, other.m_data); }

        inline UUID(uint64 x, uint64 y) { UUID_FUNC_IMPL(Constructor, m_data, x, y); }
        inline UUID(const uint8* bytes) { UUID_FUNC_IMPL(Set, m_data, bytes); }

        // Determines UUID Type and enables the specific functions.
        static void Initialize();

        // Sets the function pointers based on type.
        static void SetType(Type type);
        static inline Type GetType() { return m_type; }

        // Construct functions

        inline static void Construct(uint8* out, uint64 x, uint64 y) { UUID_FUNC_IMPL(Constructor, out, x, y); }

        // Generate functions

        inline static UUID Generate() {

            CUP_FUNCTION();

            UUID ret;
            GenerateUUID(ret.m_data);

            return ret;

        }
        inline static void Generate(UUID& out) {

            CUP_FUNCTION();

            GenerateUUID(out.m_data);

        }
        inline void Regenerate() {

            CUP_FUNCTION();

            GenerateUUID(m_data);

        }
        inline static void GenerateUUID(uint8* bytes) { UUID_FUNC_IMPL(Generate, bytes); }

        // Pretty string
        
        inline static void FromString(uint8* out, const std::string& string) {

            CUP_FUNCTION();
            FromString(out, string.c_str());

        }
        inline static void FromString(uint8* out, const char* string) { UUID_FUNC_IMPL(SetString, out, string); }

        inline static UUID FromString(const std::string& string) {

            CUP_FUNCTION();
            return FromString(string.c_str());

        }
        inline static UUID FromString(const char* string) {

            CUP_FUNCTION();

            UUID ret;
            ret.SetString(string);

            return ret;

        }
        inline void SetString(const std::string& string) {

            CUP_FUNCTION();

            SetString(string.c_str());

        }
        inline void SetString(const char* string) {

            CUP_FUNCTION();

            EnsureInitialized();

            CU_ASSERT(string != nullptr, "Invalid string passed to UUID::SetString().");
            CU_ASSERT(strlen(string) == 36, "Invalid string passed to UUID::SetString(), expected size of 36 bytes but got '{}' bytes. String: '{}'", strlen(string), string);
            CU_ASSERT(string[8] == '-' && string[13] == '-' && string[18] == '-' && string[23] == '-', "Invalid string format passed to UUID::SetString(). Expected 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx' Received: '{}'", string);

            CU_ASSERT(GetSetStringImpl() != nullptr, "UUID::GetSetStringImpl() was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type));

            GetSetStringImpl()(m_data, string);

        }

        inline static void ToString(const uint8* bytes, std::string& out) {

            CUP_FUNCTION();

            out.resize(36);
            ToString(bytes, static_cast<char*>(out.data()));

        }
        inline static void ToString(const uint8* bytes, char* out) { UUID_FUNC_IMPL(ToString, bytes, out); }

        inline std::string ToString() const {

            CUP_FUNCTION();

            std::string ret;
            ToString(ret);

            return ret;

        }
        inline void ToString(std::string& out) const {

            CUP_FUNCTION();

            out.resize(36);
            ToString((char*) out.data());

        }
        inline void ToString(char* out) const { UUID_FUNC_IMPL(ToString, m_data, out); }

        // Misc.

        inline static bool Equals(const uint8* lhs, const uint8* rhs) {

            CUP_FUNCTION();

            EnsureInitialized();

            CU_ASSERT(GetEqualsImpl() != nullptr, "UUID::GetEqualsImpl was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type));
            return GetEqualsImpl()(lhs, rhs);

        }

        inline bool IsValid() const { return *this != m_nil; }

        // Operators

        inline bool operator==(const UUID& other) const { return UUID::Equals(m_data, other.m_data); }
        inline bool operator!=(const UUID& other) const { return !(*this == other); }

        inline bool operator<(const UUID& other) const {

            CUP_FUNCTION();

            uint64* x = (uint64*) m_data;
            uint64* y = (uint64*) other.m_data;

            return *x < *y || (*x == *y && *(x + 1) < *(y + 1));

        }
        inline bool operator>(const UUID& other) const { return other < *this; }
        inline bool operator<=(const UUID& other) const { return !(*this > other); }
        inline bool operator>=(const UUID& other) const { return !(*this < other); }

        inline UUID& operator=(const UUID& other) {

            CUP_FUNCTION();

            if (&other == this) return *this;

            EnsureInitialized();
            CU_ASSERT(GetSetImpl() != nullptr, "UUID::GetSetImpl() was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type));

            GetSetImpl()(m_data, other.m_data);
            return *this;

        }

        // Getters

        inline static const UUID& GetNil() { return m_nil; }

        inline const uint8* GetBytes() const { return m_data; }

        // Misc. 

        inline size_t hash() const {

            const uint64 a = *((uint64*) m_data);
            const uint64 b = *((uint64*) &m_data[8]);
            return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));

        }

    private:
        static Type m_type;
        static const UUID m_nil;

        // UUID data

        alignas(16) uint8 m_data[16];

        inline static void EnsureInitialized() {

            static const bool unused = (Initialize(), true);
            (void) unused;

        }

        // Function implementations. If these were regular static variables, another static UUID
        // varialbe (for instance m_nil) could call Initialize, assign the pointers and then the
        // static initialization of the pointers would happen, thereby resetting them to nullptr.

        UUID_GET_IMPL(void(uint8*, const uint8*), Set);
        UUID_GET_IMPL(void(uint8*, uint64, uint64), Constructor);
        UUID_GET_IMPL(void(uint8*), Generate);
        UUID_GET_IMPL(void(uint8*, const char*), SetString);
        UUID_GET_IMPL(void(const uint8*, char*), ToString);
        UUID_GET_IMPL(bool(const uint8*, const uint8*), Equals);

    };

}

template<> struct fmt::formatter<Copper::UUID> : fmt::formatter<std::string> {

    auto format(const Copper::UUID& uuid, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "{}", uuid.ToString());

    }

};

namespace std {

    template<> struct hash<Copper::UUID> {

        size_t operator()(const Copper::UUID& uuid) const { return uuid.hash(); }

    };

}
