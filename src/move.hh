#pragma once

#include <optional>

#include "color.hh"
#include "piece-type.hh"
#include "position.hh"
#include "report-type.hh"

namespace board
{
    class Move final
    {
    public:
        using opt_piece_t = std::optional<PieceType>;

        Move(const Position& start,
             const Position& end,
             const PieceType& piece,
             const bool capture,
             const bool double_pawn_push,
             const bool queen_castling,
             const bool king_castling,
             const bool en_passant,
             const opt_piece_t& promotion = std::nullopt);

        PieceType piece_get() const;

        bool operator==(const Move& move) const
        {
             return start_ == move.start_ &&
                     end_ == move.end_ &&
                     piece_ == move.piece_ &&
                     promotion_.value() == move.promotion_.value() &&
                     capture_ == move.capture_ &&
                     double_pawn_push_ == move.double_pawn_push_ &&
                     queen_castling_ == move.queen_castling_ &&
                     king_castling_ == move.king_castling_ &&
                     en_passant_ == move.en_passant_;
        }

        bool operator!=(const Move& move) const
        {
            return !(*this == move);
        }

    private:
        // The original position of the piece
        const Position start_;

        // The destination of the piece
        const Position end_;

        // The type of the moving piece
        const PieceType piece_;

        // If promotion, contains the piecetype generated by the promotion
        const opt_piece_t promotion_;

        // If the move is a capture
        const bool capture_;

        // If the move is a double pawn push
        const bool double_pawn_push_;

        // If the move is a queen castling
        const bool queen_castling_;

        // If the move is a king castling
        const bool king_castling_;

        // If the move is an en passant
        const bool en_passant_;
    };
} // namespace board
