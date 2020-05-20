#include "evaluation.hh"
#include "utils/bits-utils.hh"

using namespace board;

namespace ai
{
    int evaluate_material(const Chessboard& board)
    {
        int evaluation = 0;
        const uint64_t white_board = board.get_board().get(Color::WHITE);
        const uint64_t black_board = board.get_board().get(Color::BLACK);

        for (auto piece : piecetype_array)
        {
            uint64_t white_piece_board = white_board
                    & board.get_board().get(piece);
            uint64_t black_piece_board = black_board
                    & board.get_board().get(piece);

            const auto piece_i = utils::utype(piece);
            const auto piece_value = piecetype_values[piece_i];

            const auto white_piece_count = utils::bits_count(white_piece_board);
            const auto black_piece_count = utils::bits_count(black_piece_board);

            evaluation += (white_piece_count - black_piece_count) * piece_value;
        }

        return evaluation;
    }

    int evaluate_squares(const Chessboard& board)
    {
        int evaluation = 0;
        const uint64_t white_board = board.get_board().get(Color::WHITE);
        const uint64_t black_board = board.get_board().get(Color::BLACK);

        for (auto piece : piecetype_array)
        {
            uint64_t white_piece_board = white_board
                    & board.get_board().get(piece);
            uint64_t black_piece_board = black_board
                    & board.get_board().get(piece);

            const auto piece_i = utils::utype(piece);
            const auto& white_piece_square_table =
                    white_piece_square_tables[piece_i];
            const auto& black_piece_square_table =
                    black_piece_square_tables[piece_i];

            int white_pos = utils::pop_lsb(white_piece_board);
            while (white_pos >= 0)
            {
                evaluation += white_piece_square_table[white_pos];
                white_pos = utils::pop_lsb(white_piece_board);
            }

            int black_pos = utils::pop_lsb(black_piece_board);
            while (black_pos >= 0)
            {
                evaluation -= black_piece_square_table[black_pos];
                black_pos = utils::pop_lsb(black_piece_board);
            }
        }

        return evaluation;
    }

    //  Result:
    // positive -> white advantage
    // negative -> black advantage
    int evaluate(const Chessboard& board)
    {
        return evaluate_material(board) + evaluate_squares(board);
    }
}
