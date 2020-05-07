#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <vector>
#include <optional>
#include <algorithm>

#include "chessboard-interface.hh"
#include "../../parsing/perft_parser/perft-parser.hh"
#include "move.hh"

// Il était tard et j'avais la flemme
// Kèstuvafér
using namespace perft_parser;

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
        Chessboard(const FenObject&);
        Chessboard(const PerftObject& perft):
            Chessboard(perft.fen_get()) {}
        Chessboard(const std::string& fen_string):
            Chessboard(parse_perft(fen_string + std::string(" w - - 0 0 0"))) {}


        std::vector<Move> generate_legal_moves();

        // Assume that move is legal
        void do_move(const Move& move);

        bool is_move_legal(const Move& move);

        bool is_check();
        bool is_check_mate();
        bool is_draw();

        opt_piece_t operator[](const Position& position) const override;

        opt_pos_t get_en_passant() const;

        bool get_white_turn() const;

        bool get_king_castling(const Color& color) const;
        bool get_queen_castling(const Color& color) const;
        
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

        bitboard_t& get_bitboard(PieceType piecetype, Color color);
        void set_position(const Position& pos, PieceType piecetype, Color color);
        void unset_position(const Position& pos, PieceType piecetype, Color color);

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
            os << rank_i + 1;
            for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
            {
                const auto file = static_cast<File>(file_i);
                const auto rank = static_cast<Rank>(rank_i);
                const auto curr_piece = board[Position(file, rank)];

                os << sep;

                if (curr_piece.has_value())
                {
                    const PieceType piecetype = curr_piece.value().first;
                    const Color piece_color = curr_piece.value().second;

                    const char piece_char = piece_to_char(piecetype);

                    os << char(piece_color == Color::WHITE ?
                               piece_char :
                               tolower(piece_char));
                }
                else
                    os << empty_cell_char;
            }

            os << std::endl;
        }

        os << sep << sep;
        for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
            os << char('A' + file_i) << sep;

        os << std::endl;

        return os;
    }
}
