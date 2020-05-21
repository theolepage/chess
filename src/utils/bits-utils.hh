#pragma once

#include <cstdint>
#include <ostream>

namespace utils
{
    inline int is_bit_set(const uint64_t bit, const int index)
    {
        return (bit >> index) & 1ULL;
    }

    inline int normalize_bit(int bit)
    {
        return bit ? 1 : 0;
    }

    inline std::ostream& print_bit(std::ostream& os, const uint64_t bit)
    {
        for (int i = 63; i >= 0; --i)
            os << normalize_bit(is_bit_set(bit, i));

        return os;
    }

    inline std::ostream& print_bitboard(std::ostream& os, const uint64_t bitboard)
    {
        constexpr int w = 8;
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < w; j++)
                os << normalize_bit(is_bit_set(bitboard, i * w + j));

            os << std::endl;
        }

        return os;
    }

    inline uint64_t north_fill(uint64_t bitboard)
    {
        bitboard |= (bitboard << 8);
        bitboard |= (bitboard << 16);
        bitboard |= (bitboard << 32);

        return bitboard;
    }

    inline uint64_t south_fill(uint64_t bitboard)
    {
        bitboard |= (bitboard >> 8);
        bitboard |= (bitboard >> 16);
        bitboard |= (bitboard >> 32);

        return bitboard;
    }

    inline uint64_t file_fill(const uint64_t bitboard)
    {
        return north_fill(bitboard) | south_fill(bitboard);
    }

    // index = 0 means set first bit
    inline void set_bit(uint64_t& bit, const int index)
    {
        bit |= 1ULL << index;
    }

    inline void unset_bit(uint64_t& bit, const int index)
    {
        bit &= ~(1ULL << index);
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
