#include "evaluation.hh"
#include "utils/bits-utils.hh"

using namespace board;

namespace ai
{
    // Result:
    // positive -> white advantage
    // negative -> black advantage
    int evaluate(const Chessboard& board)
    {
        int evaluation = 0;
        
        for (auto piece : piecetype_array)
        {
            uint64_t white_piece_board = board.get_board()(piece, Color::WHITE);
            uint64_t black_piece_board = board.get_board()(piece, Color::BLACK);

            const auto piecetype_i = utils::utype(piece);
            const auto piecetype_value = piecetype_values[piecetype_i];
            const auto& piecetype_square_table =
                    piece_square_tables[piecetype_i];

            int white_pos = utils::pop_lsb(white_piece_board);
            while (white_piece_board)
            {
                Position pos(white_pos);
                int rank_i = static_cast<int>(pos.get_rank());
                int file_i = static_cast<int>(pos.get_file());
                evaluation += piecetype_value +
                            piecetype_square_table[width - rank_i - 1][file_i];
                white_pos = utils::pop_lsb(white_piece_board);
            }

            int black_pos = utils::pop_lsb(black_piece_board);
            while (black_piece_board)
            {
                Position pos(black_pos);
                int rank_i = static_cast<int>(pos.get_rank());
                int file_i = static_cast<int>(pos.get_file());
                evaluation -= piecetype_value +
                            piecetype_square_table[rank_i][file_i];
                black_pos = utils::pop_lsb(black_piece_board);
            }
        }

        return evaluation;
    }
}
