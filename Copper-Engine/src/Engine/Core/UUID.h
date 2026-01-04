#pragma once

#include "Engine/Core/Core.h"

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir 

#define UUID_FUNC_IMPL(func, ...) CUP_FUNCTION(); CU_ASSERT(func != nullptr, "UUID::" #func " was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type)); func(__VA_ARGS__)

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

        inline UUID(const UUID& other) { UUID_FUNC_IMPL(m_setImpl, m_data, other.m_data); }

        inline UUID(uint64 x, uint64 y) { UUID_FUNC_IMPL(m_constructorImpl, m_data, x, y); }
        inline UUID(const uint8* bytes) { UUID_FUNC_IMPL(m_setImpl, m_data, bytes); }

        // Determines UUID Type and enables the specific functions.
        static void Initialize();

        // Sets the function pointers based on type.
        static void SetType(Type type);
        static inline Type GetType() { return m_type; }

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
        inline static void GenerateUUID(uint8* bytes) { UUID_FUNC_IMPL(m_generateImpl, bytes); }

        // Pretty string
        
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

            CU_ASSERT(string != nullptr, "Invalid string passed to UUID::SetString().");
            CU_ASSERT(strlen(string) == 36, "Invalid string passed to UUID::SetString(), expected size of 36 bytes but got '{}' bytes. String: '{}'", strlen(string), string);
            CU_ASSERT(string[8] == '-' && string[13] == '-' && string[18] == '-' && string[23] == '-', "Invalid string format passed to UUID::SetString(). Expected 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx' Received: '{}'", string);

            CU_ASSERT(m_setStrigImpl != nullptr, "UUID::m_setStrigImpl was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type));

            m_setStrigImpl(m_data, string);

        }

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
        inline void ToString(char* out) const { UUID_FUNC_IMPL(m_toStringImpl, m_data, out); }

        inline bool IsValid() const { return *this != m_nil; }

        // Operators

        inline bool operator==(const UUID& other) const {

            CU_ASSERT(m_equalsImpl != nullptr, "UUID::m_equalsImpl was not assigned. UUID Type: '{}'", static_cast<uint32>(m_type));
            return m_equalsImpl(m_data, other.m_data);

        }
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

            UUID_FUNC_IMPL(m_setImpl, m_data, other.m_data);
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

        // Function implementations. These pointers will change based on 
        // the available SIMD instructions (or lack there of). These are automatically
        // set in UUID::SetType()

        static std::function<void(uint8*, const uint8*)> m_setImpl;
        static std::function<void(uint8*, uint64, uint64)> m_constructorImpl;
        static std::function<void(uint8*)> m_generateImpl;
        static std::function<void(uint8*, const char*)> m_setStrigImpl;
        static std::function<void(const uint8*, char*)> m_toStringImpl;
        static std::function<bool(const uint8*, const uint8*)> m_equalsImpl;

        // UUID data

        alignas(16) uint8 m_data[16];

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
