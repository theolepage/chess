#pragma once

#include "fen-object.hh"
#include "chess_engine/board/chessboard.hh"

namespace perft_parser
{
    class PerftObject
    {
    public:
        PerftObject(const FenObject &f, int depth)
        {
            fen_ = f;
            depth_ = depth;
        }

        FenObject get_fen(void) const
        {
            return fen_;
        }

        int get_depth(void) const
        {
            return depth_;
        }

        // For test purpose
        bool operator==(const PerftObject& rhs) const
        {
            return fen_ == rhs.fen_ && depth_ == rhs.depth_;
        }

        bool operator!=(const PerftObject& rhs) const
        {
            return !(*this == rhs);
        }

        board::Chessboard get_board() const
        {
            return board::Chessboard(); // TODO replace with alan implem
        }

    private:
        FenObject fen_;
        int depth_;
    };
}