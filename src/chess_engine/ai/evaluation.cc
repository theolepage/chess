#include "evaluation.hh"

using namespace board;

namespace ai
{
    unsigned get_point_value(const Chessboard& board, const Color color)
    {
        unsigned point_value = 0;

        for (const auto& piecetype : piecetype_array)
        {
            const auto piecetype_i = utils::utype(piecetype);
            point_value += piecetype_value[piecetype_i] * board.get_bitboard_count(piecetype, color);
        }

        return point_value;
    }

    // Result:
    // positive -> white advantage
    // negative -> black advantage
    int evaluate(const Chessboard& board)
    {
        return get_point_value(board, Color::WHITE) - get_point_value(board, Color::BLACK);
    }
}
