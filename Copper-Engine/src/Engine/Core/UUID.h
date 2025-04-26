#pragma once

#include "Engine/Core/Core.h"

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir 

namespace Copper {

    struct UUID {

    public:
        // Creates a new random UUID
        UUID() = default;

        UUID(const UUID& other);

        UUID(uint64 x, uint64 y);
        UUID(const uint8* bytes);

        explicit UUID(const std::string& bytes);

        // Generate functions

        inline static UUID Generate() {

            CUP_FUNCTION();

            UUID ret;
            Generate(ret);

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

        inline std::string ToBytes() const {

            CUP_FUNCTION();

            std::string ret;
            ToBytes(ret);

            return ret;

        }
        inline void ToBytes(std::string& out) const {

            CUP_FUNCTION();

            out.resize(sizeof(m_data));
            ToBytes((char*) out.data());

        }
        void ToBytes(char* out) const;

        // Pretty string
        
        inline static UUID CreateFromString(const std::string& string) {

            CUP_FUNCTION();
            return CreateFromString(string.c_str());

        }
        inline static UUID CreateFromString(const char* string) {

            CUP_FUNCTION();

            UUID ret;
            ret.SetString(string);

            return ret;

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
        bool operator<(const UUID& other) const;
        UUID& operator=(const UUID& other);

        inline bool operator!=(const UUID& other) const { return !(*this == other); }
        inline bool operator>(const UUID& other) const { return other < *this; }
        inline bool operator<=(const UUID& other) const { return !(*this > other); }
        inline bool operator>=(const UUID& other) const { return !(*this < other); }

        // Misc. 

        inline size_t hash() const {

            const uint64 a = *((uint64*) m_data);
            const uint64 b = *((uint64*) &m_data[8]);
            return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));

        }

        inline static const UUID& GetInvalid() { return m_invalid; }

    private:

        alignas(128) uint8 m_data[16];

        static const UUID m_invalid;

        static void GenerateUUID(uint8* bytes);

    };

    inline std::ostream& operator<<(std::ostream& stream, const UUID& uuid) {

        CUP_FUNCTION();

        std::string tmp;
        uuid.ToString(tmp);

        return stream << tmp;

    }
    inline std::istream& operator>>(std::istream& stream, UUID& uuid) {

        CUP_FUNCTION();

        std::string tmp;
        stream >> tmp;

        uuid.SetString(tmp.c_str());
        return stream;

    }

}

namespace std {

    template<> struct hash<Copper::UUID> {

        size_t operator()(const Copper::UUID& uuid) const { return uuid.hash(); }

    };

}
