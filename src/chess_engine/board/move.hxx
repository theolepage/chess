namespace board
{
    inline Move::Move(const Position& start,
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

    inline bool Move::operator==(const Move& move) const
    {
        if (promotion_.has_value() != move.promotion_.has_value())
            return false;
        bool almost_equal = start_ == move.start_ &&
            end_ == move.end_ &&
            piece_ == move.piece_ &&
            capture_ == move.capture_ &&
            double_pawn_push_ == move.double_pawn_push_ &&
            queen_castling_ == move.queen_castling_ &&
            king_castling_ == move.king_castling_ &&
            en_passant_ == move.en_passant_;
        if (promotion_.has_value())
            return almost_equal && promotion_.value() == move.promotion_.value();
        return almost_equal;
    }

    inline bool Move::operator!=(const Move& move) const
    {
        return !(*this == move);
    }

    inline const Position& Move::start_get() const
    {
        return start_;
    }

    inline const Position& Move::end_get() const
    {
        return end_;
    }

    inline PieceType Move::piece_get() const
    {
        return piece_;
    }

    inline bool Move::capture_get() const
    {
        return capture_;
    }

    inline bool Move::double_pawn_push_get() const
    {
        return double_pawn_push_;
    }

    inline bool Move::queen_castling_get() const
    {
        return queen_castling_;
    }

    inline bool Move::king_castling_get() const
    {
        return king_castling_;
    }

    inline bool Move::en_passant_get() const
    {
        return en_passant_;
    }

    inline const Move::opt_piece_t& Move::promotion_get() const
    {
        return promotion_;
    }
}
