#include "cupch.h"
#include "UUID.cs.h"
#include "Engine/Core/UUID.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <endianness.h>

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#include <random>
#include <limits>
#include <memory>

// Copy of the code found in Engine/Core/UUID.cpp file which is a slightly modified version
// of the uuid_v4 library found at https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid directory.

namespace Copper {

    extern void  m128itos(__m128i x, char* mem);
    extern __m128i stom128i(const char* mem);

}

namespace Copper::Scripting::UUID {

    typedef ::Copper::UUID UUID;

    static std::shared_ptr<std::mt19937_64> generator = std::make_shared<std::mt19937_64>(std::random_device()());
    static std::uniform_int_distribution<uint64> distribution = std::uniform_int_distribution<uint64>(std::numeric_limits<uint64>::min(), std::numeric_limits<uint64>::max());

    void Constructor(MonoArray* data, uint64 x, uint64 y) {

        CUP_FUNCTION();

        uint8* bytes = mono_array_addr(data, uint8, 0);

        __m128i z = _mm_set_epi64x(x, y);
        _mm_storeu_si128((__m128i*) bytes, z);

    }

    void GenerateUUID(MonoArray* data) {

        CUP_FUNCTION();

        uint8* bytes = mono_array_addr(data, uint8, 0);

        const __m128i andMask = _mm_set_epi64x(0xFFFFFFFFFFFFFF3Full, 0xFF0FFFFFFFFFFFFFull);
        const __m128i orMask = _mm_set_epi64x(0x0000000000000080ull, 0x0040000000000000ull);

        __m128i n = _mm_set_epi64x(distribution(*generator), distribution(*generator));
        __m128i uuid = _mm_or_si128(_mm_and_si128(n, andMask), orMask);

        _mm_storeu_si128((__m128i*) bytes, uuid);

    }

    void SetBytes(MonoArray* data, MonoArray* bytes) {

        CUP_FUNCTION();

        CU_ASSERT(data != nullptr && bytes != nullptr, "Invalid arrays passed to UUID::Internal_SetBytes.");
        CU_ASSERT(mono_array_length(data) == 16, "Invalid array length for the uuid data array.");
        CU_ASSERT(mono_array_length(bytes) == 16, "Invalid array length for the bytes array.");

        void* src = mono_array_addr(bytes, uint8, 0);
        void* dst = mono_array_addr(data, uint8, 0);

        memcpy(dst, src, 16);

    }

    void SetString(MonoArray* data, MonoString* string) {

        CUP_FUNCTION();

        CU_ASSERT(string != nullptr, "Invalid string passed to UUID::SetString.");
        if (mono_string_length(string)!= 36) {

            mono_raise_exception(mono_get_exception_argument("string", "Invalid UUID string format passed to UUID::ToString(). Expected format: 9b5217e4-a7f8-4ef5-acaa-786577c18640"));
            return;
            
        }

        uint8* bytes = mono_array_addr(data, uint8, 0);

        char* tmp = mono_string_to_utf8(string);
        _mm_storeu_si128((__m128i*) bytes, stom128i(tmp));

        mono_free(tmp);

    }
    MonoString* ToString(MonoArray* data) {

        CUP_FUNCTION();

        uint8* bytes = mono_array_addr(data, uint8, 0);
        char out[36];

        __m128i x = _mm_loadu_si128((__m128i*) bytes);
        m128itos(x, out);

        return mono_string_new_len(AppDomain(), out, 36);

    }

    bool Equals(MonoArray* a, MonoArray* b) {

        CUP_FUNCTION();

        __m128i x = _mm_loadu_si128((__m128i*) mono_array_addr(a, uint8, 0));
        __m128i y = _mm_loadu_si128((__m128i*) mono_array_addr(b, uint8, 0));

        __m128i neq = _mm_xor_si128(x, y);
        return _mm_test_all_zeros(neq, neq);

    }
    bool LessThan(MonoArray* a, MonoArray* b) {

        CUP_FUNCTION();

        uint64* x = (uint64*) mono_array_addr(a, uint8, 0);
        uint64* y = (uint64*) mono_array_addr(b, uint8, 0);
        
        return *x < *y || (*x == *y && *(x + 1) < *(y + 1));

    }

    int32 Hash(MonoArray* data) {

        CUP_FUNCTION();

        uint8* bytes = mono_array_addr(data, uint8, 0);

        const uint64 a = *((uint64*) bytes);
        const uint64 b = *((uint64*) &bytes[8]);
        const uint64 ret = a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));

        // Let's hope that this folding won't fuck up anything in the future.
        // Why the fuck does C# use ints for hashes and not size_t like a normal sane language.
        return static_cast<int32>(ret ^ (ret >> 32));

    }

}
