#include <cstdint>
#include <optional>
#include <cassert>
#include <memory>

#include "move-initialization.hh"

namespace board
{
    MoveInitialization MoveInitialization::instance_ = MoveInitialization();

    MoveInitialization::MoveInitialization()
    {
        init_rays();

        init_rook_masks();
        init_bishop_masks();

        init_bishop_attacks();
        init_rook_attacks();
    }

    MoveInitialization& MoveInitialization::get_instance()
    {
        return instance_;
    }

    uint64_t MoveInitialization::get_targets(
            const PieceType& piece,
            int pos,
            uint64_t blockers
    )
    {
        if (piece == PieceType::BISHOP)
        {
            blockers &= bishop_masks[pos];
            const uint64_t key = (blockers * defs::bishop_magics[pos])
                            >> (defs::NB_POS - defs::bishop_bits[pos]);
            return bishop_attacks[pos][key];
        }
        if (piece == PieceType::ROOK)
        {
            blockers &= rook_masks[pos];
            const uint64_t key = (blockers * defs::rook_magics[pos])
                            >> (defs::NB_POS - defs::rook_bits[pos]);
            return rook_attacks[pos][key];
        }
        if (piece == PieceType::QUEEN)
        {
            return get_targets(PieceType::ROOK, pos, blockers)
                    | get_targets(PieceType::BISHOP, pos, blockers);
        }
        assert(false);
        return 0ULL;
    }

    // Returns a mask containing set bits on the axis
    uint64_t MoveInitialization::get_ray(
        const Position& from, const int file, const int rank)
    {
        uint64_t res = 0ULL;
        std::optional<Position> pos = from.translate(file, rank);
        while (pos.has_value())
        {
            res |= 1ULL << pos->get_index();
            pos = pos->translate(file, rank);
        }
        return res;
    }

    void MoveInitialization::init_rays(void)
    {
        for (int i = 0; i < defs::NB_POS; ++i)
        {
            Position from(i);
            rays[0][i] = get_ray(from, -1,  0); // line left
            rays[1][i] = get_ray(from,  1,  0); // line right
            rays[2][i] = get_ray(from,  0,  1); // line up
            rays[3][i] = get_ray(from,  0, -1); // line down
            rays[4][i] = get_ray(from, -1,  1); // diagonal up left
            rays[5][i] = get_ray(from,  1,  1); // diagonal up right
            rays[6][i] = get_ray(from, -1, -1); // diagonal down left
            rays[7][i] = get_ray(from,  1, -1); // diagonal down right
        }
    }

    void MoveInitialization::init_rook_masks(void)
    {
        for (int i = 0; i < defs::NB_POS; i++)
        {
            rook_masks[i] = (rays[0][i]
                            | rays[1][i]
                            | rays[2][i]
                            | rays[3][i])
                            & ~(defs::borders);
        }
    }

    void MoveInitialization::init_bishop_masks(void)
    {
        for (int i = 0; i < defs::NB_POS; i++)
        {
            bishop_masks[i] = (rays[4][i]
                                | rays[5][i]
                                | rays[6][i]
                                | rays[7][i])
                                & ~(defs::borders);
        }
    }

    /**
    * @short Gerate for this specific index & mask the blocker mask
    * @param index of the current blocker
    * @param mask of possible move of the current piece
    * @return the mask representing the current blocker
    * 
    * We need a way to generate a unique mask of blockers or each blocker
    * To do that we are going to use the bit representation of index
    * To generate all those possibilites the for loops goes on all the set
    * bits of the current mask
    * For each of those set bits we get their position and remove them
    * from the mask
    * We then only set it at the correct pos if it intersect with the index
    * By using the index bit representation we generate all the possiblities
    * and uniquely for all the blockers possiblities
    */
    uint64_t MoveInitialization::get_blockers(const int blocker_index,
                                    uint64_t current_mask)
    {
        const int nb_bits = utils::bits_count(current_mask);
        uint64_t current_blocker = 0ULL;

        for (int i = 0; i < nb_bits; i++)
        {
            const int bit_position = utils::pop_lsb(current_mask);
            if (blocker_index & (1ULL << i))
                current_blocker |= (1ULL << bit_position);
        }
        return current_blocker;
    }
    
    uint64_t MoveInitialization::get_bishop_attacks_classical(
        int pos, uint64_t blockers)
    {
        uint64_t res = 0ULL;

        for (int diag = 4; diag < 8; diag++)
        {
            uint64_t ray = rays[diag][pos];
            res |= ray;

            uint64_t blockers_on_ray = ray & blockers;

            // We going south we want the search in the opposite direction
            int nearest_blocker = ((diag < 6)
                    ? utils::bit_scan_forward(blockers_on_ray)
                    : utils::bit_scan_reverse(blockers_on_ray));
            if (nearest_blocker == -1)
                continue;

            uint64_t ray_before_blocker = rays[diag][nearest_blocker];
            res &= ~ray_before_blocker;
        }

        return res;
    }

    uint64_t MoveInitialization::get_rook_attacks_classical(
        int pos, uint64_t blockers)
    {
        uint64_t res = 0ULL;

        for (int diag = 0; diag < 4; diag++)
        {
            uint64_t ray = rays[diag][pos];
            res |= ray;

            uint64_t blockers_on_ray = ray & blockers;

            // We going south we want the search in the opposite direction
            int nearest_blocker = ((diag < 2)
                    ? utils::bit_scan_forward(blockers_on_ray)
                    : utils::bit_scan_reverse(blockers_on_ray));
            if (nearest_blocker == -1)
                continue;

            uint64_t ray_before_blocker = rays[diag][nearest_blocker];
            res &= ~ray_before_blocker;
        }

        return res;
    }

    // For each position of the bishop (64) we went to generate all blockers
    // The is a finite number of blockers for each position
    // This number is stored in the bishop_bits array
    void MoveInitialization::init_bishop_attacks(void)
    {
        for (int pos = 0; pos < defs::NB_POS; pos++)
        {
            for (int b = 0; b < (1 << defs::bishop_bits[pos]); b++)
            {
                const uint64_t blockers = get_blockers(b, bishop_masks[pos]);
                const int key = (blockers * defs::bishop_magics[pos])
                                    >> (64 - defs::bishop_bits[pos]);
                bishop_attacks[pos][key] = get_bishop_attacks_classical(
                                                                pos,
                                                                blockers);
            }
        }
    }

    void MoveInitialization::init_rook_attacks(void)
    {
        for (int pos = 0; pos < defs::NB_POS; pos++)
        {
            for (int b = 0; b < (1 << defs::rook_bits[pos]); b++)
            {
                const uint64_t blockers = get_blockers(b, rook_masks[pos]);
                const int key = (blockers * defs::rook_magics[pos])
                                    >> (defs::NB_POS - defs::rook_bits[pos]);
                rook_attacks[pos][key] = get_rook_attacks_classical(pos,
                                                                blockers);
            }
        }
    }
}