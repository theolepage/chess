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

        friend std::ostream& operator<<(std::ostream& os, const Chessboard& board);

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

    /*
     * White pieces are displayed with uppercase and
     * black pieces with lowercase
     */
    inline std::ostream& operator<<(std::ostream& os, const Chessboard& board)
    {
        constexpr std::string_view sep = " ";

        for (int rank_i = Chessboard::width - 1; rank_i >= 0; rank_i--)
        {
            std::cout << rank_i + 1 << sep;
            for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
            {
                const auto file = static_cast<File>(file_i);
                const auto rank = static_cast<Rank>(rank_i);
                const auto curr_piece = board[Position(file, rank)];

                if (curr_piece.has_value())
                {
                    const PieceType piecetype = curr_piece.value().first;
                    const Color piece_color = curr_piece.value().second;

                    const char piecetype_char = piecetype_to_char(piecetype);

                    std::cout << char(piece_color == Color::WHITE ?
                                      piecetype_char :
                                      tolower(piecetype_char));
                }
                else
                {
                    std::cout << empty_cell_char;
                }

                std::cout << sep;
            }

            std::cout << std::endl;
        }

        std::cout << sep << sep;
        for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
            std::cout << char('A' + file_i) << sep;

        std::cout << std::endl;

        return os;
    }
}
