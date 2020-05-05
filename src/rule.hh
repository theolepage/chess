#pragma once

#include <vector>

#include "chessboard.hh"
#include "move.hh"

namespace rule
{

    std::vector<board::Move> generate_pawn_moves(const board::Chessboard& board);
    std::vector<board::Move> generate_king_moves(const board::Chessboard& board);
    std::vector<board::Move> generate_bishop_moves(const board::Chessboard& board);
    std::vector<board::Move> generate_rook_moves(const board::Chessboard& board);
    std::vector<board::Move> generate_queen_moves(const board::Chessboard& board);
    std::vector<board::Move> generate_knight_moves(const board::Chessboard& board);

} // namespace rule