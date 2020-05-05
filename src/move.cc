#include "move.hh"

namespace board
{
    Move::Move(const Position& start,
               const Position& end,
               PieceType piece,
               bool capture,
               bool double_pawn_push,
               bool queen_castling,
               bool king_castling,
               bool en_passant,
               const opt_piece_t& promotion)
        : start_(start)
        , end_(end)
        , piece_(piece)
        , promotion_(promotion)
        , capture_(capture)
        , double_pawn_push_(double_pawn_push)
        , queen_castling_(queen_castling)
        , king_castling_(king_castling)
    {}

    PieceType Move::piece_get() const
    {
        return piece_;
    }
} // namespace board