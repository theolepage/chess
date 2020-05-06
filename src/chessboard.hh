#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <vector>
#include <optional>
#include <algorithm>

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
        using opt_pos_t = std::optional<Position>;

        Chessboard();

        std::vector<Move> generate_legal_moves();

        // Assume that move is legal
        void do_move(const Move& move);

        bool is_move_legal(const Move& move);

        bool is_check();
        bool is_check_mate();
        bool is_draw();

        opt_pos_t en_passant_get();
        bool king_castling_get(Color color);
        bool queen_castling_get(Color color);

        opt_piece_t operator[](const Position& pos) const override;

        friend std::ostream& operator<<(std::ostream& os, const Chessboard& board);

    private:
        std::array<bitboard_t, nb_pieces> white_bitboards_;
        std::array<bitboard_t, nb_pieces> black_bitboards_;

        bool white_turn_;

        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;

        opt_pos_t en_passant_;

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

                    const char piece_char = piece_to_char(piecetype);

                    std::cout << char(piece_color == Color::WHITE ?
                                      piece_char :
                                      tolower(piece_char));
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
