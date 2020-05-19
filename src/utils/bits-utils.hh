#pragma once

#include <cstdint>

namespace utils
{
    inline int is_bit_set(const uint64_t bit, const int index)
    {
        return (bit >> index) & 1ULL;
    }

    // Get the least significant bit position and remove it from n
    inline int pop_lsb(uint64_t& n)
    {
        const int result = __builtin_ffsll(n) - 1;
        n &= n - 1;
        return result;
    }

    // Returns the number of bits in the number
    inline int bits_count(const uint64_t n)
    {
        return __builtin_popcountll(n);
    }

    // Returns the position of the least significant bit
    inline int bit_scan_forward(const uint64_t n)
    {
        if (n == 0ULL)
            return -1;
        return __builtin_ffsll(n) - 1;
    }

    inline int bit_scan_reverse(const uint64_t n)
    {
        if (n == 0ULL)
            return -1;
        return 63 - __builtin_clzll(n);
    }
}