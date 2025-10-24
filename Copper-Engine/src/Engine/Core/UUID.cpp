#include "cupch.h"
#include "UUID.h"

#include <endianness.h>

#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#include <random>
#include <limits>
#include <memory>

// This is a modified version of the uuid_v4 library found at
// https://github.com/crashoz/uuid_v4
// License can be found in the lib/uuid dir

namespace Copper {

    static std::shared_ptr<std::mt19937_64> generator = std::make_shared<std::mt19937_64>(std::random_device()());
    static std::uniform_int_distribution<uint64> distribution = std::uniform_int_distribution<uint64>(std::numeric_limits<uint64>::min(), std::numeric_limits<uint64>::max());

    const UUID UUID::m_invalid = UUID(0, 0);

    void  m128itos(__m128i x, char* mem);
    __m128i stom128i(const char* mem);

    UUID::UUID(const UUID& other) {

        CUP_FUNCTION();

        __m128i x = _mm_load_si128((__m128i*) other.m_data);
        _mm_store_si128((__m128i*) m_data, x);

    }
    UUID::UUID(uint64 x, uint64 y) {

        CUP_FUNCTION();

        __m128i z = _mm_set_epi64x(x, y);
        _mm_store_si128((__m128i*) m_data, z);

    }
    UUID::UUID(const uint8* bytes) {

        CUP_FUNCTION();

        __m128i x = _mm_loadu_si128((__m128i*) bytes);
        _mm_store_si128((__m128i*) m_data, x);

    }

    UUID::UUID(const std::string& bytes) {

        CUP_FUNCTION();

        __m128i x = betole128(_mm_loadu_si128((__m128i*) bytes.data()));
        _mm_store_si128((__m128i*) m_data, x);

    }

    // Generation

    void UUID::GenerateUUID(uint8* bytes) {

        CUP_FUNCTION();

        const __m128i andMask = _mm_set_epi64x(0xFFFFFFFFFFFFFF3Full, 0xFF0FFFFFFFFFFFFFull);
        const __m128i orMask = _mm_set_epi64x(0x0000000000000080ull, 0x0040000000000000ull);

        __m128i n = _mm_set_epi64x(distribution(*generator), distribution(*generator));
        __m128i uuid = _mm_or_si128(_mm_and_si128(n, andMask), orMask);

        _mm_store_si128((__m128i*) bytes, uuid);

    }

    // Byte string

    void UUID::ToBytes(char* out) const {

        CUP_FUNCTION();

        __m128i x = betole128(_mm_load_si128((__m128i*) m_data));
        _mm_storeu_si128((__m128i*) out, x);

    }

    // Pretty string

    void UUID::SetString(const char* string) {

        CUP_FUNCTION();

        _mm_store_si128((__m128i*) m_data, stom128i(string));

    }

    void UUID::ToString(char* out) const {

        CUP_FUNCTION();

        __m128i x = _mm_load_si128((__m128i*) m_data);
        m128itos(x, out);

    }

    // Operators

    bool UUID::operator==(const UUID& other) const {

        CUP_FUNCTION();

        __m128i x = _mm_load_si128((__m128i*) m_data);
        __m128i y = _mm_load_si128((__m128i*) other.m_data);

        __m128i neq = _mm_xor_si128(x, y);
        return _mm_test_all_zeros(neq, neq);

    }
    bool UUID::operator<(const UUID& other) const {

        CUP_FUNCTION();

        uint64* x = (uint64*) m_data;
        uint64* y = (uint64*) other.m_data;
        
        return *x < *y || (*x == *y && *(x + 1) < *(y + 1));

    }

    UUID& UUID::operator=(const UUID& other) {

        CUP_FUNCTION();

        if (&other == this) return *this;

        __m128i x = _mm_load_si128((__m128i*) other.m_data);
        _mm_store_si128((__m128i*) m_data, x);

        return *this;

    }

    // SIMD functions

    void m128itos(__m128i x, char* mem) {

        CUP_FUNCTION();

        // Expand each byte in x to two bytes in res
        // i.e. 0x12345678 -> 0x0102030405060708
        // Then translate each byte to its hex ascii representation
        // i.e. 0x0102030405060708 -> 0x3132333435363738
        const __m256i mask = _mm256_set1_epi8(0x0F);
        const __m256i add = _mm256_set1_epi8(0x06);
        const __m256i alpha_mask = _mm256_set1_epi8(0x10);
        const __m256i alpha_offset = _mm256_set1_epi8(0x57);

        __m256i a = _mm256_castsi128_si256(x);
        __m256i as = _mm256_srli_epi64(a, 4);
        __m256i lo = _mm256_unpacklo_epi8(as, a);
        __m128i hi = _mm256_castsi256_si128(_mm256_unpackhi_epi8(as, a));
        __m256i c =  _mm256_inserti128_si256(lo, hi, 1);
        __m256i d = _mm256_and_si256(c, mask);
        __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(_mm256_add_epi8(d, add), alpha_mask), 3);
        __m256i offset = _mm256_blendv_epi8(_mm256_slli_epi64(add, 3), alpha_offset, alpha);
        __m256i res = _mm256_add_epi8(d, offset);

        // Add dashes between blocks as specified in RFC-4122
        // 8-4-4-4-12
        const __m256i dash_shuffle = _mm256_set_epi32(0x0b0a0908, 0x07060504, 0x80030201, 0x00808080, 0x0d0c800b, 0x0a090880, 0x07060504, 0x03020100);
        const __m256i dash = _mm256_set_epi64x(0x0000000000000000ull, 0x2d000000002d0000ull, 0x00002d000000002d, 0x0000000000000000ull);

        __m256i resd = _mm256_shuffle_epi8(res, dash_shuffle);
        resd = _mm256_or_si256(resd, dash);

        _mm256_storeu_si256((__m256i*)mem, betole256(resd));
        *(uint16_t*)(mem+16) = betole16(_mm256_extract_epi16(res, 7));
        *(uint32_t*)(mem+32) = betole32(_mm256_extract_epi32(res, 7));

    }
    __m128i stom128i(const char* mem) {

        CUP_FUNCTION();

        // Remove dashes and pack hex ascii bytes in a 256-bits int
        const __m256i dash_shuffle = _mm256_set_epi32(0x80808080, 0x0f0e0d0c, 0x0b0a0908, 0x06050403, 0x80800f0e, 0x0c0b0a09, 0x07060504, 0x03020100);

        __m256i x = betole256(_mm256_loadu_si256((__m256i*)mem));
        x = _mm256_shuffle_epi8(x, dash_shuffle);
        x = _mm256_insert_epi16(x, betole16(*(uint16_t*)(mem+16)), 7);
        x = _mm256_insert_epi32(x, betole32(*(uint32_t*)(mem+32)), 7);

        // Build a mask to apply a different offset to alphas and digits
        const __m256i sub = _mm256_set1_epi8(0x2F);
        const __m256i mask = _mm256_set1_epi8(0x20);
        const __m256i alpha_offset = _mm256_set1_epi8(0x28);
        const __m256i digits_offset = _mm256_set1_epi8(0x01);
        const __m256i unweave = _mm256_set_epi32(0x0f0d0b09, 0x0e0c0a08, 0x07050301, 0x06040200, 0x0f0d0b09, 0x0e0c0a08, 0x07050301, 0x06040200);
        const __m256i shift = _mm256_set_epi32(0x00000000, 0x00000004, 0x00000000, 0x00000004, 0x00000000, 0x00000004, 0x00000000, 0x00000004);

        // Translate ascii bytes to their value
        // i.e. 0x3132333435363738 -> 0x0102030405060708
        // Shift hi-digits
        // i.e. 0x0102030405060708 -> 0x1002300450067008
        // Horizontal add
        // i.e. 0x1002300450067008 -> 0x12345678
        __m256i a = _mm256_sub_epi8(x, sub);
        __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(a, mask), 2);
        __m256i sub_mask = _mm256_blendv_epi8(digits_offset, alpha_offset, alpha);
        a = _mm256_sub_epi8(a, sub_mask);
        a = _mm256_shuffle_epi8(a, unweave);
        a = _mm256_sllv_epi32(a, shift);
        a = _mm256_hadd_epi32(a, _mm256_setzero_si256());
        a = _mm256_permute4x64_epi64(a, 0b00001000);

        return _mm256_castsi256_si128(a);

    }

}
