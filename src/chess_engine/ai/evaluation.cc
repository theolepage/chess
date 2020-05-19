#include "evaluation.hh"

using namespace board;

namespace ai
{
    // Result:
    // positive -> white advantage
    // negative -> black advantage
    int evaluate(const Chessboard&)
    {
        int evaluation = 0;

        // for (auto piecetype : piecetype_array)
        // {
        //     const bitboard_t& white_bitboard =
        //             board.get_bitboard(piecetype, Color::WHITE);
        //     const bitboard_t& black_bitboard =
        //             board.get_bitboard(piecetype, Color::BLACK);

        //     const auto piecetype_i = utils::utype(piecetype);
        //     const auto piecetype_value = piecetype_values[piecetype_i];
        //     const auto& piecetype_square_table =
        //             piece_square_tables[piecetype_i];

        //     for (size_t rank_i = 0; rank_i < width; rank_i++)
        //     {
        //         for (size_t file_i = 0; file_i < width; file_i++)
        //         {
        //             if (white_bitboard[rank_i][file_i])
        //                 evaluation += piecetype_value +
        //                     piecetype_square_table[width - rank_i][file_i];

        //             if (black_bitboard[rank_i][file_i])
        //                 evaluation -= piecetype_value +
        //                     piecetype_square_table[rank_i][file_i];
        //         }
        //     }
        // }

        return evaluation;
    }
}
