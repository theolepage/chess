#pragma once

#include <cstdint>

#include "position.hh"

namespace board
{
    class magic_biboard
    {
    private:
        static constexpr auto NB_POSITIONS = 64;
        static constexpr auto DIRECTIONS = 64;
        //static uint64_t non_sliding_attacks[2][6][64] = {{{0}}}; // Not rook, bishop ect

        static uint64_t rook_masks[NB_POSITIONS] = {0}; // Where rook can move based on the 64 possible positions
        static uint64_t bishop_masks[NB_POSITIONS] = {0}; // Where bishop can move based on the 64 possible positions

        static uint64_t rook_attack_sets[NB_POSITIONS][4096] = {{0}}; // All possible attack set of the rook
        static uint64_t bishop_attack_sets[NB_POSITIONS][1024] = {{0}}; // All possible attack set of the bishop

        static uint64_t rays[DIRECTIONS][NB_POSITIONS] = {{0}};

        static constexpr uint64_t RANK_ONE = 0xffull;
        static constexpr uint64_t RANK_TWO = 0xff00ull;
        static constexpr uint64_t RANK_THREE = 0xff0000ull;
        static constexpr uint64_t RANK_FOUR = 0xff000000ull;
        static constexpr uint64_t RANK_FIVE = 0xff00000000ull;
        static constexpr uint64_t RANK_SIX = 0xff0000000000ull;
        static constexpr uint64_t RANK_SEVEN = 0xff000000000000ull;
        static constexpr uint64_t RANK_EIGHT = 0xff00000000000000ull;

        static constexpr uint64_t FILE_A = 0x101010101010101ull;
        static constexpr uint64_t FILE_B = 0x202020202020202ull;
        static constexpr uint64_t FILE_C = 0x404040404040404ull;
        static constexpr uint64_t FILE_D = 0x808080808080808ull;
        static constexpr uint64_t FILE_E = 0x1010101010101010ull;
        static constexpr uint64_t FILE_F = 0x2020202020202020ull;
        static constexpr uint64_t FILE_G = 0x4040404040404040ull;
        static constexpr uint64_t FILE_H = 0x8080808080808080ull;

        static constexpr uint64_t borders = FILE_A | FILE_H | RANK_1 | RANK_8;

        uint64_t get_ray(const Position& from,
                        const int file,
                        const int rank)
        {
            std::optional<Position> pos = from.move(file, rank);
            uint64_t ray = 0ULL;
            while (pos)
            {
                ray |= 1ULL << pos.get_index();
                pos = pos->move(file, rank);
            }
            return ray;
        }

        void init_rays(void)
        {
            for (int i = 0; i < NB_POSITIONS; ++i)
            {
                Position from(i);
                // Rook / Queen
                rays[0][i] = get_ray(res, from, -1,  0);   // line left
                rays[1][i] = get_ray(res, from,  1,  0);   // line right
                rays[2][i] = get_ray(res, from,  0,  1);   // line up
                rays[3][i] = get_ray(res, from,  0, -1);   // line down

                // Bishop / Queen
                rays[4][i] = get_ray(res, from, -1,  1);   // diagonal up left
                rays[5][i] = get_ray(res, from,  1,  1);   // diagonal up right
                rays[6][i] = get_ray(res, from, -1, -1);   // diagonal down left
                rays[7][i] = get_ray(res, from,  1, -1);   // diagonal down right
            }
        }

        void init_rook_masks(void)
        {
            for (int i = 0; i < NB_POSITIONS; i++)
            {
                rook_masks[i] = rays[0][i]
                                | rays[1][i]
                                | rays[2][i]
                                | rays[3][i]
                                & ~(borders);
            }
        }

        void init_bishop_masks(void)
        {
            for (int i = 0; i < NB_POSITIONS; i++)
            {
                bishop_masks[i] = rays[4][i]
                                | rays[5][i]
                                | rays[6][i]
                                | rays[7][i]
                                & ~(borders);
            }
        }

        void init_possible_attack(void)
        {
            //initPawnAttacks();
            //initKnightAttacks();
            //initKnightAttacks();
            init_rook_masks();
            init_bishop_masks();
        }

        void init_bishop_magic_bitboard(void)
        {
        // For all squares
        for (int square = 0; square < 64; square++) {
            // For all possible blockers for this square
            for (int blockerIndex = 0; blockerIndex < (1 << _bishopIndexBits[square]); blockerIndex++) {
            U64 blockers = _getBlockersFromIndex(blockerIndex, _bishopMasks[square]);
            _bishopTable[square][(blockers * _bishopMagics[square]) >> (64 - _bishopIndexBits[square])] =
                _getBishopAttacksSlow(square, blockers);
            }
        }

        void init_magic_bitboards(void)
        {
            init_bishop_magic_bitboard();
            init_rook_magic_bitboard();
        }

    public:
        static void init_all_set(void)
        {
            init_rays();

            init_possible_attack();

            init_magic_bitboards();
        }

    };
}