#pragma once

#include <vector>
#include <optional>

#include "chessboard-interface.hh"
#include "move.hh"

namespace board
{
    enum class Axis
    {
        VERTICAL,
        HORIZONTAL
    };

    class Chessboard : public ChessboardInterface
    {
    public:
        using side_piece_t = std::pair<PieceType, Color>;
        using opt_piece_t = std::optional<side_piece_t>;

        std::vector<Move> generate_legal_moves();

        void do_move(const Move& move);

        bool is_move_legal(const Move& move);

        bool is_check();

        bool is_check_mate();

        bool is_draw();

        opt_piece_t operator[](const Position& position) const;

        std::optional<Position> get_en_passant() const;

        bool get_king_castling(const Color& color) const;
        bool get_queen_castling(const Color& color) const;

    private:
        bool white_turn_;

        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;

        // FIXME en_passant;
        std::optional<Position> en_passant_;

        unsigned turn_;
        unsigned last_fifty_turn_;
    };
}
