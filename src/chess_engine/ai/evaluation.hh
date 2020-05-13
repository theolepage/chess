#pragma once

#include "chess_engine/board/chessboard.hh"

namespace ai
{
    constexpr size_t width = board::Chessboard::width;
    constexpr size_t nb_pieces = board::nb_pieces;

    int evaluate(const board::Chessboard& board);
}
