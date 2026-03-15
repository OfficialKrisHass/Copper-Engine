#include "cupch.h"
#include "UUID.h"

#include <random>
#include <memory>

// This file uses SSE4.1 only in a single place whereas all the other functions are SSE2
// so pcs that support SSE2 but not SSE4.1 won't have access to this, which is kinda stupid.
// But I am too lazy to do this, plus this doesn't seem to be an issue on most pcs, but it should be changed.
#include <emmintrin.h>
#include <smmintrin.h>

namespace Copper {

    extern std::shared_ptr<std::mt19937_64> generator;
    extern std::uniform_int_distribution<uint64> distribution;

    void UUIDSet_SSE(uint8* data, const uint8* otherData) {

        CUP_FUNCTION();

        __m128i x = _mm_loadu_si128((__m128i*) otherData);
        _mm_store_si128((__m128i*) data, x);

    }
    void UUIDConstructor_SSE(uint8* data, uint64 x, uint64 y) {

        CUP_FUNCTION();

        __m128i z = _mm_set_epi64x(x, y);
        _mm_store_si128((__m128i*) data, z);

    }

    void UUIDGenerate_SSE(uint8* bytes) {

        CUP_FUNCTION();

        const __m128i andMask = _mm_set_epi64x(0xFFFFFFFFFFFFFF3Full, 0xFF0FFFFFFFFFFFFFull);
        const __m128i orMask = _mm_set_epi64x(0x0000000000000080ull, 0x0040000000000000ull);

        __m128i n = _mm_set_epi64x(distribution(*generator), distribution(*generator));
        __m128i uuid = _mm_or_si128(_mm_and_si128(n, andMask), orMask);

        _mm_store_si128((__m128i*) bytes, uuid);

    }

    bool UUIDEquals_SSE(const uint8* lhs, const uint8* rhs) {

        CUP_FUNCTION();

        __m128i x = _mm_load_si128((__m128i*) lhs);
        __m128i y = _mm_load_si128((__m128i*) rhs);

        __m128i neq = _mm_xor_si128(x, y);
        return _mm_test_all_zeros(neq, neq);

    }

}
