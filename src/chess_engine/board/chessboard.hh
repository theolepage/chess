#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <vector>
#include <optional>
#include <algorithm>
#include <unordered_map>

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
// ekip
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
        
        Chessboard(const std::string& str, const Color& color):
            Chessboard(parse_perft(str + ((color == Color::WHITE) ? std::string(" w - - 0 0 0") : std::string(" b - - 0 0 0")))) {}
        
        Chessboard(const PerftObject& perft):
            Chessboard(perft.get_fen()) {}
        
        Chessboard(const std::string& fen_string):
            Chessboard(parse_perft(fen_string + std::string(" w - - 0 0 0"))) {}

        static char sidepiece_to_char(const PieceType piecetype, const Color color);
        static char sidepiece_to_char(const side_piece_t& sidepiece);
        std::string to_fen_string(void) const;

        const bitboard_t& get_bitboard(const PieceType piecetype, const Color color) const;
        bitboard_t& get_bitboard(const PieceType piecetype, const Color color);
        size_t get_bitboard_count(const PieceType piecetype, const Color color) const;

        std::vector<Move> generate_legal_moves(void);

        // return the same value than generate_legal_moves().empty(),
        // without generating superfluous moves
        bool has_legal_moves(void);

        // Assume that move is legal
        void do_move(const Move& move);
        //void undo_move(const Move& move, const option_parser::BoardState& state);

        bool is_move_legal(const Move& move);
        bool is_possible_move_legal(const Move& move) const;
        bool is_move_possible(const Move& move);

        bool is_check(void);
        bool is_checkmate(void);
        bool is_pat(void);
        bool threefold_repetition(void);
        bool is_draw(void);

        opt_piece_t operator[](const Position& position) const override;

        opt_pos_t get_en_passant() const;

        bool get_white_turn() const;
        void set_white_turn(bool state);

        bool get_king_castling(const Color& color) const;
        bool get_queen_castling(const Color& color) const;
        void set_king_castling(const Color& color, bool state);
        void set_queen_castling(const Color& color, bool state);

        Position get_king_position(void) const;

        friend std::ostream& operator<<(std::ostream& os, const Chessboard& board);

        bool operator==(const Chessboard& rhs) const
        {
            return white_king_castling_ == rhs.white_king_castling_
            && white_queen_castling_ == rhs.white_queen_castling_
            && black_king_castling_ == rhs.black_king_castling_
            && black_queen_castling_ == rhs.black_queen_castling_
            && white_bitboards_ == rhs.white_bitboards_
            && black_bitboards_ == rhs.black_bitboards_
            && white_turn_ == rhs.white_turn_
            && en_passant_ == rhs.en_passant_
            && turn_ == rhs.turn_
            && last_fifty_turn_ == rhs.last_fifty_turn_;
        }

    private:
        /**
         * First access white or black
         * Then access the piece
         * Then the line
         * Then the row
        */
        std::array<bitboard_t, nb_pieces> white_bitboards_;
        std::array<bitboard_t, nb_pieces> black_bitboards_;

        // a hash table which key corresponds to a fen string representing
        // a state of the board and which value is the number of occurences
        // of this state during the game
        std::unordered_map<std::string, short> state_count_;

        bool white_turn_;

        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;

        opt_pos_t en_passant_;

        unsigned turn_;
        unsigned last_fifty_turn_;

        std::ostream& write_fen_rank(std::ostream& os, const Rank rank) const;
        std::ostream& write_fen_board(std::ostream& os) const;

        void register_state();

        void set_piece(const Position& pos, const PieceType piecetype, const Color color);
        void unset_piece(const Position& pos, const PieceType piecetype, const Color color);
        void move_piece(const Position& start, const Position& end, const PieceType piecetype, const Color color);
        void change_piece_type(const Position& pos, const PieceType old_type, const PieceType new_type, const Color color);

        void init_end_ranks(const PieceType piecetype, const File file);
        void symetric_init_end_ranks(const PieceType piecetype, const File file);

        unsigned get_point_value(const Color color) const;

        void register_double_pawn_push(const Move& move, const Color color);
        void forget_en_passant(void);
        void update_last_fifty_turn(const Move& move);
        void eat_en_passant(const Move& move, const Color color);
        void move_castling_rook(const Move& move, const Color color);
        void update_white_castling_bools(const Move& move);
        void update_black_castling_bools(const Move& move);
        void update_castling_bools(const Move& move, const Color color);
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

                    os << Chessboard::sidepiece_to_char(piecetype, piece_color);
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
