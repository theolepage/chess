#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <vector>
#include <optional>

#include "chessboard-interface.hh"
#include "move.hh"

namespace board
{
    class Chessboard : public ChessboardInterface
    {
    public:
        constexpr static size_t width = 8;

        /*
         * the board is represented by width lines
         * a line is represented by a bitset (of width bits)
         *
         * each bit is set to 1 if there is a piece at its corresponding
         * position or 0 otherwise
         */
        using bitboard_t = std::array<std::bitset<width>, width>;

        Chessboard();

        std::vector<Move> generate_legal_moves();

        void do_move(const Move& move);

        bool is_move_legal(const Move& move);

        bool is_check();

        bool is_check_mate();

        bool is_draw();

        opt_piece_t operator[](const Position& position);


    private:
        std::array<bitboard_t, nb_pieces> white_bitboards;
        std::array<bitboard_t, nb_pieces> black_bitboards;

        bool white_turn_;

        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;

        // FIXME en_passant;

        unsigned turn_;
        unsigned last_fifty_turn_;

        void init_end_ranks(PieceType piecetype, File file);
        void symetric_init_end_ranks(PieceType piecetype, File file);
    };
}
