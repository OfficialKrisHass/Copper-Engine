#pragma once

#include "Engine/Core/Core.h"

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir 

namespace Copper {

    struct UUID {

    public:
        // Defaults to the invalid UUID (all zeroes)
        UUID() : UUID(0, 0) {}

        UUID(const UUID& other);

        UUID(uint64 x, uint64 y);
        UUID(const uint8* bytes);

        explicit UUID(const std::string& bytes);

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

        // Byte string

        void ToBytes(char* out) const;

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
        void SetString(const char* string);

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
        void ToString(char* out) const;

        inline bool IsValid() const { return *this != m_invalid; }

        // Operators

        bool operator==(const UUID& other) const;
        inline bool operator!=(const UUID& other) const { return !(*this == other); }

        bool operator<(const UUID& other) const;
        inline bool operator>(const UUID& other) const { return other < *this; }
        inline bool operator<=(const UUID& other) const { return !(*this > other); }
        inline bool operator>=(const UUID& other) const { return !(*this < other); }

        UUID& operator=(const UUID& other);

        // Misc. 

        inline size_t hash() const {

            const uint64 a = *((uint64*) m_data);
            const uint64 b = *((uint64*) &m_data[8]);
            return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));

        }

        inline static const UUID& GetInvalid() { return m_invalid; }

    private:

        alignas(16) uint8 m_data[16];

        static const UUID m_invalid;

        static void GenerateUUID(uint8* bytes);

    };

}

template<> struct fmt::formatter<Copper::UUID> : fmt::formatter<std::string> {

    auto format(const Copper::UUID& uuid, format_context& ctx) -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "{}", uuid.ToString());

    }

};

namespace std {

    template<> struct hash<Copper::UUID> {

        size_t operator()(const Copper::UUID& uuid) const { return uuid.hash(); }

    };

}
