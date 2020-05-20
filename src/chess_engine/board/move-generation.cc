#include <vector>

#include "rule.hh"
#include "chessboard.hh"
#include "entity/move.hh"
#include "utils/utype.hh"
#include "utils/bits-utils.hh"
#include "move-initialization.hh"

using namespace board;

namespace move_generation
{
    std::vector<Move> generate_bishop_moves(const Chessboard& board)
    {
        std::vector<Move> res;

        // Get all bishops of color currently playing
        const PieceType piece = PieceType::BISHOP;
        const Color color = board.get_white_turn()
            ? Color::WHITE : Color::BLACK;
        const Color opponent_color = board.get_white_turn()
            ? Color::BLACK : Color::WHITE;
        uint64_t pieces = board.get_board()(piece, color);

        // Iterate over all bishops of color currently playing
        int pos = utils::pop_lsb(pieces);
        while (pieces)
        {
            uint64_t targets = MoveInitialization::get_instance().get_targets(
                piece,
                pos,
                board.get_board()()
            );

            // Remove pieces from player currently playing
            targets &= ~board.get_board()(color);

            // Extract non captures moves and captures moves
            uint64_t captures = targets & board.get_board()(opponent_color);
            uint64_t non_captures = targets & ~board.get_board()(opponent_color);

            // Handle capture moves
            int capture_dest = utils::pop_lsb(captures);
            while (captures)
            {
                // Create capture move
                res.emplace_back(
                    Position(pos),
                    Position(capture_dest),
                    piece,
                    true
                );
                capture_dest = utils::pop_lsb(captures);
            }

            // Handle simple moves
            int non_capture_dest = utils::pop_lsb(non_captures);
            while (non_captures)
            {
                // Create simple move
                res.emplace_back(
                    Position(pos),
                    Position(non_capture_dest),
                    piece
                );
                non_capture_dest = utils::pop_lsb(non_captures);
            }

            pos = utils::pop_lsb(pieces);
        }

        return res;
    }

    std::vector<Move> generate_rook_moves(const Chessboard& board);
    std::vector<Move> generate_queen_moves(const Chessboard& board);
} // namespace move_generation