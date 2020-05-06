#pragma once

#include <utility>
#include <cctype>
#include <cassert>

#include "utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum class File : uint8_t
    {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
    };

    /* The rank enum represent the lines
     * on the board */
    enum class Rank : uint8_t
    {
        ONE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
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
            rank_ =  static_cast<Rank>(rank - '0');
        }

        File file_get() const;
        Rank rank_get() const;

        bool operator==(const Position& pos) const;
        bool operator!=(const Position& pos) const;

    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"
