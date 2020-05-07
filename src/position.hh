#pragma once

#include <utility>
#include <cctype>
#include <cassert>
#include <iostream>

#include "utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum class File : uint8_t
    {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,
        G = 6,
        H = 7,
    };

    /* The rank enum represent the lines
     * on the board */
    enum class Rank : uint8_t
    {
        ONE = 0,
        TWO = 1,
        THREE = 2,
        FOUR = 3,
        FIVE = 4,
        SIX = 5,
        SEVEN = 6,
        EIGHT = 7,
    };

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        Position(File file, Rank rank);

        Position(char file, char rank)
        {
            assert(islower(file) && isdigit(rank));
            file_ = static_cast<File>(file - 'a');
            rank_ =  static_cast<Rank>(rank - '0' - 1); // + 1 cause ONE corresponds to 0
        }

        File get_file() const;
        Rank get_rank() const;

        bool operator==(const Position& pos) const;
        bool operator!=(const Position& pos) const;

        std::optional<Position> move(int file, int rank) const;

    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"