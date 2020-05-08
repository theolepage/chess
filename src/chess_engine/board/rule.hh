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
    bool have_pieces_between(const Chessboard& board,
                              const Position& x,
                              const Position& y);

    std::vector<Position> get_pieces_positions(const Chessboard& board,
                                               const PieceType& piece,
                                               const Color& color);

    std::vector<Position> get_positions_between(const Position& x,
                                                const Position& y);
    
    std::vector<Position> get_authorized_pos(const PieceType& piece,
                                             const Position& from);

    std::optional<Move> get_possible_move(const Chessboard& board,
                                          const PieceType& piece,
                                          const Color& color,
                                          const Position& from,
                                          const Position& to);

    std::optional<Move> register_castling(const Chessboard& board,
                                          const Color& color,
                                          bool king_castling);

    bool register_promotion(std::vector<Move>& moves,
                            const Position& from,
                            const Position& to,
                            const Color& color,
                            bool capture);

    std::vector<Move> generate_moves(const Chessboard& board,
                                     const PieceType& piece);

    std::vector<Move> generate_all_moves(const Chessboard& board);

    std::vector<Move> generate_pawn_moves(const Chessboard& board);
    std::vector<Move> generate_king_moves(const Chessboard& board);
    std::vector<Move> generate_bishop_moves(const Chessboard& board);
    std::vector<Move> generate_rook_moves(const Chessboard& board);
    std::vector<Move> generate_queen_moves(const Chessboard& board);
    std::vector<Move> generate_knight_moves(const Chessboard& board);
} // namespace rule
