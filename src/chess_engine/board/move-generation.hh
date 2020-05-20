#pragma once

#include <vector>

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/entity/move.hh"
#include "chess_engine/board/entity/piece-type.hh"
#include "chess_engine/board/entity/color.hh"
#include "chess_engine/board/entity/position.hh"

using namespace board;

namespace move_generation
{
    std::vector<Move> generate_bishop_moves(const Chessboard& board);
    std::vector<Move> generate_rook_moves(const Chessboard& board);
    std::vector<Move> generate_queen_moves(const Chessboard& board);
} // namespace move_generation