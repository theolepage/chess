#include "move-generation.hh"

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
    static std::vector<Move> generate_moves(
            const PieceType& piece,
            const Chessboard& board
    )
    {
        std::vector<Move> res;

        // Get all piece of color currently playing
        const Color color = board.get_white_turn()
            ? Color::WHITE : Color::BLACK;
        const Color opponent_color = board.get_white_turn()
            ? Color::BLACK : Color::WHITE;
        uint64_t pieces = board.get_board()(piece, color);

        // Iterate over all piece of color currently playing
        int pos = utils::pop_lsb(pieces);
        while (pos >= 0)
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
            while (capture_dest >= 0)
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
            while (non_capture_dest >= 0)
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

    std::vector<Move> generate_bishop_moves(const Chessboard& board)
    {
        return generate_moves(PieceType::BISHOP, board);
    }

    std::vector<Move> generate_rook_moves(const Chessboard& board)
    {
        return generate_moves(PieceType::ROOK, board);
    }

    std::vector<Move> generate_queen_moves(const Chessboard& board)
    {
        return generate_moves(PieceType::QUEEN, board);
    }

    std::vector<Move> generate_knight_moves(const Chessboard& board)
    {
        return generate_moves(PieceType::KNIGHT, board);
    }

    std::vector<Move> generate_king_moves(const Chessboard& board)
    {
        std::vector<Move> res = generate_moves(PieceType::KING, board);

        // Handle castling
        const Color color = board.get_white_turn()
                ? Color::WHITE : Color::BLACK;
        const Rank rank = (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT;

        // FIXME: Beter ordering of conditions to not do useless
        // FIME: Make sure correct masks values
        if (board.get_king_castling(color))
        {
            uint64_t have_pieces_between = board.get_board()()
                & (color == Color::WHITE ? 0x60 : 0x6000000000000000);
            uint64_t attacked = board.pos_threatened(Position(File::E, rank))
                                | board.pos_threatened(Position(File::F, rank))
                                | board.pos_threatened(Position(File::G, rank));
            if (!have_pieces_between && !attacked)
            {
                Move king_castling(Position(File::E, rank),
                                   Position(File::G, rank),
                                   PieceType::KING);
                king_castling.set_king_castling(true);
                res.push_back(king_castling);
            }
        }

        if (board.get_queen_castling(color))
        {
            uint64_t have_pieces_between = board.get_board()()
                & (color == Color::WHITE ? 0xE : 0xE00000000000000);
            uint64_t attacked = board.pos_threatened(Position(File::E, rank))
                                | board.pos_threatened(Position(File::D, rank))
                                | board.pos_threatened(Position(File::C, rank));
            if (!have_pieces_between && !attacked)
            {
                Move queen_castling(Position(File::E, rank),
                                   Position(File::C, rank),
                                   PieceType::QUEEN);
                queen_castling.set_queen_castling(true);
                res.push_back(queen_castling);
            }
        }

        return res;
    }

    std::vector<Move> generate_pawn_moves(const Chessboard&)
    {
        // FIXME: Handle pawn
        return std::vector<Move>();
    }
} // namespace move_generation