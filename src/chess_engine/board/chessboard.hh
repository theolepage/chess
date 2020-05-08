#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <vector>
#include <optional>
#include <algorithm>

#include "chessboard-interface.hh"
#include "parsing/perft_parser/perft-parser.hh"
#include "parsing/perft_parser/fen-object.hh"
#include "parsing/perft_parser/perft-object.hh"
#include "move.hh"
#include "parsing/option_parser/option-parser.hh"
#include "position.hh"
#include "piece-type.hh"
#include "color.hh"

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
            Chessboard(perft.get_fen()) {}
        Chessboard(const std::string& fen_string):
            Chessboard(parse_perft(fen_string + std::string(" w - - 0 0 0"))) {}


        std::vector<Move> generate_legal_moves(void) const;

        // Assume that move is legal
        void do_move(const Move& move);
        void undo_move(const Move& move, const option_parser::BoardState& state);

        bool is_move_legal(const Move& move) const;

        bool is_check(void);
        bool is_check_mate(void);
        bool is_draw(void);

        opt_piece_t operator[](const Position& position) const override;

        opt_pos_t get_en_passant() const;

        bool get_white_turn() const;

        bool get_king_castling(const Color& color) const;
        bool get_queen_castling(const Color& color) const;

        Position get_king_position(void) const;
        
        friend std::ostream& operator<<(std::ostream& os, const Chessboard& board);

    private:
        /**
         * First access white or black
         * Then access the piece
         * Then the line
         * Then the row
        */
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

        const bitboard_t& get_bitboard(PieceType piecetype, Color color) const;
        bitboard_t& get_bitboard(PieceType piecetype, Color color);
        void set_position(const Position& pos, PieceType piecetype, Color color);
        void unset_position(const Position& pos, PieceType piecetype, Color color);

        void init_end_ranks(PieceType piecetype, File file);
        void symetric_init_end_ranks(PieceType piecetype, File file);
        void set_piece(const PieceType& piece_type, const Color& color, const Position& move);

        bool is_move_possible(const Move& move) const;
        bool is_possible_move_legal(const Move& move) const;
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

        os << sep;
        for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
            os << sep << char('A' + file_i);

        return os;
    }
}
