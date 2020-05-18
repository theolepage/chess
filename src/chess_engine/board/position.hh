#pragma once

#include <utility>
#include <cassert>
#include <iostream>

#include "utils/utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum class File
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H
    };

    /* The rank enum represent the lines
     * on the board */
    enum class Rank
    {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT
    };

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        // (x, y)
        Position(File file, Rank rank);

        Position(uint8_t x, uint8_t y)
        {
            file_ = static_cast<File>(x);
            rank_ =  static_cast<Rank>(y);
        }

        Position(int i)
        {
            assert(i >= 0 && i < 64);
            file_ = static_cast<File>(i % 8);
            rank_ =  static_cast<Rank>(i / 8);
        }

        Position(char file, char rank)
        {
            assert(islower(file) && isdigit(rank));
            file_ = static_cast<File>(file - 'a');
            rank_ =  static_cast<Rank>(rank - '0' - 1); // + 1 cause ONE corresponds to 0
        }
        Position& operator=(const Position& position) = default;

        File get_file() const;
        Rank get_rank() const;
        int get_index(void) const;

        bool operator==(const Position& pos) const;
        bool operator!=(const Position& pos) const;

        std::optional<Position> move(int file, int rank) const;

        friend std::ostream& operator<<(std::ostream& os, const Position& pos);

    private:
        File file_; // col / x
        Rank rank_; // line / y
    };

} // namespace board

#include "position.hxx"