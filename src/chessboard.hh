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

        opt_piece_t operator[](const Position& pos) const override
        {
            size_t rank_i = utils::utype(pos.rank_get());
            size_t file_i = utils::utype(pos.file_get());

            for (auto piecetype : piecetype_array)
            {
                const auto piecetype_i = utils::utype(piecetype);
                const bitboard_t white_piecetype_bitboard = white_bitboards[piecetype_i];
                const bitboard_t black_piecetype_bitboard = black_bitboards[piecetype_i];

                if (white_piecetype_bitboard[rank_i][file_i])
                    return std::make_pair(piecetype, Color::WHITE);

                if (black_piecetype_bitboard[rank_i][file_i])
                    return std::make_pair(piecetype, Color::BLACK);
            }

            return std::nullopt;
        }

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
