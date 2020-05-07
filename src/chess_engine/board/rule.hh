#pragma once

#include <vector>

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/move.hh"
#include "chess_engine/board/piece-type.hh"
#include "chess_engine/board/color.hh"
#include "chess_engine/board/position.hh"

using namespace board;

namespace rule
{
    std::vector<Position> get_pieces_positions(const Chessboard& board,
                                               const PieceType& piece,
                                               const Color& color);

    std::vector<Position> get_positions_between(const Position& x,
                                                const Position& y);

    int count_pieces_between(const Chessboard& board,
                             const Position& x,
                             const Position& y);

    std::vector<Move> generate_pawn_moves(const Chessboard& board);
    std::vector<Move> generate_king_moves(const Chessboard& board);
    std::vector<Move> generate_bishop_moves(const Chessboard& board);
    std::vector<Move> generate_rook_moves(const Chessboard& board);
    std::vector<Move> generate_queen_moves(const Chessboard& board);
    std::vector<Move> generate_knight_moves(const Chessboard& board);
} // namespace rule