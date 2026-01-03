#include "cupch.h"
#include "UUID.h"

#include <endianness.h>

#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#include <random>
#include <limits>
#include <memory>

#if defined(_MSC_VER)
  #include <intrin.h>
#else
  #include <cpuid.h>
#endif

// Platform independent cpuid implementation.
// out are the eax, ebx, ecx and edx registers.
static void cpuid(int out[4], int leaf, int count) {

#if defined(_MSC_VER)
    __cpuidex(out, leaf, count);
#else
    __cpuid_count(leaf, count, out[0], out[1], out[2], out[3]);
#endif

}

// Platform independent xgetbv implementation.
// Returns the xcr0 register
static int xgetbv() {

#if defined(_MSC_VER)
    return _xgetbv(0);
#else
    uint32_t eax, edx;
    __asm__ volatile ("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
    return ((uint64_t)edx << 32) | eax;
#endif

}

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir

namespace Copper {

    // Used for the generation itself

    std::shared_ptr<std::mt19937_64> generator = std::make_shared<std::mt19937_64>(std::random_device()());
    std::uniform_int_distribution<uint64> distribution = std::uniform_int_distribution<uint64>(std::numeric_limits<uint64>::min(), std::numeric_limits<uint64>::max());

    // UUID static variables

    UUID::Type UUID::m_type = UUID::Type::None;
    const UUID UUID::m_invalid = UUID(0, 0);

    std::function<void(uint8*, const uint8*)> UUID::m_setImpl = nullptr;
    std::function<void(uint8*, uint64, uint64)> UUID::m_constructorImpl = nullptr;
    std::function<void(uint8*)> UUID::m_generateImpl = nullptr;
    std::function<void(const uint8*, char*)> UUID::m_toBytesImpl = nullptr;
    std::function<void(uint8*, const char*)> UUID::m_setStrigImpl = nullptr;
    std::function<void(const uint8*, char*)> UUID::m_toStringImpl = nullptr;
    std::function<bool(const uint8*, const uint8*)> UUID::m_equalsImpl = nullptr;

    // UUID Functions implementations.
    //
    // Scalar functions found in UUID.cpp (here)
    // SSE functions found in UUID_SSE4.cpp
    // AVX functions found in UUID_AVX2.cpp

    // SSE

    extern void UUIDSet_SSE(uint8* data, const uint8* otherData);
    extern void UUIDConstructor_SSE(uint8* data, uint64 x, uint64 y);

    extern void UUIDGenerate_SSE(uint8* bytes);

    extern void UUIDToBytes_SSE(const uint8* data, char* out);

    extern bool UUIDEquals_SSE(const uint8* lhs, const uint8* rhs);

    // AVX

    extern void UUIDSetString_AVX(uint8* data, const char* string);
    extern void UUIDToString_AVX(const uint8* data, char* out);

    // Scalar implementations.

    void UUIDSet(uint8* data, const uint8* otherData);
    void UUIDConstructor(uint8* data, uint64 x, uint64 y);

    void UUIDGenerate(uint8* bytes);

    void UUIDToBytes(const uint8* data, char* out);

    bool UUIDEquals(const uint8* lhs, const uint8* rhs);

    void UUIDSetString(uint8* data, const char* string);
    void UUIDToString(const uint8* data, char* out);

    // Query functions

    static bool SSE4Support();
    static bool AVX2Support();

    void UUID::Init() {

        CUP_FUNCTION();

        // AVX2 also checks for SSE support so no need to check for both

        if (AVX2Support())
            m_type = Type::SIMD_AVX2;
        else if (SSE4Support())
            m_type = Type::SIMD_SSE4;
        else
            m_type = Type::Scalar;

        SetType(m_type);

    }

    void UUID::SetType(Type type) {

        CUP_FUNCTION();

        using enum UUID::Type;

        switch (type) {

            case Type::Scalar: {

                // No SIMD instructions availalbe means we have to do it all the slow way.

                m_setImpl = UUIDSet;
                m_constructorImpl = UUIDConstructor;
                m_generateImpl = UUIDGenerate;
                m_toBytesImpl = UUIDToBytes;
                m_equalsImpl = UUIDEquals;

                m_setStrigImpl = UUIDSetString;
                m_toStringImpl = UUIDToString;

                break;

            }
            case Type::SIMD_SSE4: {

                // SSE4.1 is what we need for 99% of the implementation, however
                // the Pretty string functions require AVX, so we have to use the scalar
                // versions here.

                m_setImpl = UUIDSet_SSE;
                m_constructorImpl = UUIDConstructor_SSE;
                m_generateImpl = UUIDGenerate_SSE;
                m_toBytesImpl = UUIDToBytes_SSE;
                m_equalsImpl = UUIDEquals_SSE;

                m_setStrigImpl = UUIDSetString;
                m_toStringImpl = UUIDToString;

                break;

            }
            case Type::SIMD_AVX2: {

                // AVX2 support enables both SSE, and fills in the gaps of the
                // pretty string functions, which is why it's so similar to the
                // SSE4 type.

                m_setImpl = UUIDSet_SSE;
                m_constructorImpl = UUIDConstructor_SSE;
                m_generateImpl = UUIDGenerate_SSE;
                m_toBytesImpl = UUIDToBytes_SSE;
                m_equalsImpl = UUIDEquals_SSE;

                m_setStrigImpl = UUIDSetString_AVX;
                m_toStringImpl = UUIDToString_AVX;

                break;

            }
            default: {

                CU_ASSERT(false, "Invalid UUID type '{}'", static_cast<uint32>(type));
                break;

            }

        }

    }

    bool SSE4Support() {

        // Leaf 1 count 0 = Processor info and feature bits
        // ECX bit 19 = SSE4.1 support

        int out[4];
        cpuid(out, 1, 0);

        return (out[2] & (1 << 19)) != 0;

    }
    bool AVX2Support() {

        // AVX
        // The CPU may support AVX instructions, but if the OS does not support
        // extended state saving (xsave and xrestore), the SIMD instructions will
        // crash. Which is why this is a bit more complex.

        // Leaf 1 count 0 = Processor info and feature bits
        // ECX bit 27 = xsave and xrestore support
        // ECX bit 28 = AVX support

        int out[4];
        cpuid(out, 1, 0);

        bool cpu = (out[2] & (1 << 28)) != 0; // AVX. CPU dependant
        bool xsave = (out[2] & (1 << 27)) != 0; // xsave and xrestore support. OS dependant

        if (!cpu || !xsave)
            return false;

        // XCR = Extended Control Register. Determines which register sets are enabled
        // Bit 1 = SSE support
        // Bit 2 = AVX support

        int xcr = xgetbv();

        // Test for both SSE and AVX support (0x6 = first and second bit)
        if ((xcr & 0x6) != 0x6) return false;

        // AVX2

        // Leaf 7 count 0 = Extended features
        // EBX bit 5 = AVX support

        cpuid(out, 7, 0);
        return (out[1] & (1 << 5)) != 0;

    }

    // Scalar function implementations.

}
