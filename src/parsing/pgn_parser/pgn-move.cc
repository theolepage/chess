#include "pgn-move.hh"

namespace board
{
    PgnMove::PgnMove(const Position& start, const Position& end,
                     PieceType piece, bool capture, ReportType report,
                     const PgnMove::opt_piece_t& promotion)
        : start_(start)
        , end_(end)
        , piece_(piece)
        , promotion_(promotion)
        , capture_(capture)
        , report_(report)
    {}

    PgnMove PgnMove::generate_castling(bool queen_side, Color color)
    {
        static const Position wking_pos{File::E, Rank::ONE};
        static const Position bking_pos{File::E, Rank::EIGHT};
        static const PgnMove w_small{wking_pos,
                                     {File::G, Rank::ONE},
                                     PieceType::KING,
                                     false,
                                     ReportType::NONE};
        static const PgnMove w_big{wking_pos,
                                   {File::C, Rank::ONE},
                                   PieceType::KING,
                                   false,
                                   ReportType::NONE};
        static const PgnMove b_small{bking_pos,
                                     {File::G, Rank::EIGHT},
                                     PieceType::KING,
                                     false,
                                     ReportType::NONE};
        static const PgnMove b_big{bking_pos,
                                   {File::C, Rank::EIGHT},
                                   PieceType::KING,
                                   false,
                                   ReportType::NONE};

        if (color == Color::WHITE)
            return queen_side ? w_big : w_small;

        return queen_side ? b_big : b_small;
    }

    void PgnMove::report_set(ReportType report)
    {
        report_ = report;
    }


} // namespace board
