#include "move.hh"

namespace board
{
    Move::Move(const Position& start,
               const Position& end,
               const PieceType& piece,
               const bool capture,
               const bool double_pawn_push,
               const bool queen_castling,
               const bool king_castling,
               const bool en_passant,
               const opt_piece_t& promotion)
        : start_(start)
        , end_(end)
        , piece_(piece)
        , promotion_(promotion)
        , capture_(capture)
        , double_pawn_push_(double_pawn_push)
        , queen_castling_(queen_castling)
        , king_castling_(king_castling)
        , en_passant_(en_passant)
    {}

    PieceType Move::piece_get() const
    {
        return piece_;
    }
} // namespace board