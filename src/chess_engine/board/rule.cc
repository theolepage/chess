// To-Do: implement is_king_checked(Color, Position) => bool
// - if king of color is checked => only move king
// - king cannot put himself in check situation
// - castling

// To-Do: add unit tests

#include <vector>
#include <cmath>

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/move.hh"
#include "chess_engine/board/rule.hh"

using namespace board;

namespace rule
{
    std::vector<Position> get_pieces_positions(const Chessboard& board,
                                               const PieceType& piece,
                                               const Color& color)
    {
        std::vector<Position> res;

        for (size_t file = 0; file < Chessboard::width; file++)
        {
            for (size_t rank = 0; rank < Chessboard::width; rank++)
            {
                Position pos(static_cast<File>(file), static_cast<Rank>(rank));
                if (board[pos]->first == piece && board[pos]->second == color)
                    res.push_back(pos);
            }
        }

        return res;
    }

    std::vector<Position> get_positions_between(const Position& x,
                                                const Position& y)
    {
        std::vector<Position> res;
        int x_file = static_cast<int>(x.get_file());
        int x_rank = static_cast<int>(x.get_rank());
        int y_file = static_cast<int>(y.get_file());
        int y_rank = static_cast<int>(y.get_rank());

        // Determine shift_file and shift_rank
        int shift_file = (y_file - x_file) / abs(y_file - x_file);
        int shift_rank = (y_rank - x_rank) / abs(y_rank - x_rank);

        res.push_back(x);
        std::optional<Position> pos = x.move(shift_file, shift_rank);
        while (pos && pos != y)
        {
            res.push_back(*pos);
            pos = pos->move(shift_file, shift_rank);
        }
        res.push_back(y);

        return res;
    }

    int count_pieces_between(const Chessboard& board,
                             const Position& x,
                             const Position& y)
    {
        int res = 0;
        for (Position pos : get_positions_between(x, y))
            if (board[pos])
                res++;
        return res;
    }

    static void register_pos(std::vector<Position>& v,
                             std::vector<std::optional<Position>> positions)
    {
        for (auto pos : positions)
            if (pos)
                v.push_back(*pos);
    }

    static void register_pos_line(std::vector<Position>& v,
                                  const Position& from,
                                  int file,
                                  int rank)
    {
        std::optional<Position> pos = from.move(file, rank);
        while (pos)
        {
            v.push_back(*pos);
            pos = pos->move(file, rank);
        }
    }

    std::vector<Position> get_authorized_pos(const PieceType& piece,
                                             const Position& from)
    {
        std::vector<Position> res;

        if (piece == PieceType::KING)
        {
            register_pos(res, {
                from.move(-1,  1), from.move(0,  1), from.move(1,  1), // top
                from.move(-1,  0), from.move(1,  0),                   // center
                from.move(-1, -1), from.move(0, -1), from.move(1, -1)  // bottom
            });
        }
        if (piece == PieceType::KNIGHT)
        {
            register_pos(res, {
                from.move(-1,  2), from.move(-2,  1),   // top left
                from.move( 1,  2), from.move( 2,  1),   // top right
                from.move(-1, -2), from.move(-2, -1),   // bottom left
                from.move( 1, -2), from.move( 2, -1)    // bottom right
            });
        }
        if (piece == PieceType::ROOK || piece == PieceType::QUEEN)
        {
            register_pos_line(res, from, -1,  0);   // line left
            register_pos_line(res, from,  1,  0);   // line right
            register_pos_line(res, from,  0,  1);   // line up
            register_pos_line(res, from,  0, -1);   // line down
        }
        if (piece == PieceType::BISHOP || piece == PieceType::QUEEN)
        {
            register_pos_line(res, from, -1,  1);   // diagonal up left
            register_pos_line(res, from,  1,  1);   // diagonal up right
            register_pos_line(res, from, -1, -1);   // diagonal down left
            register_pos_line(res, from,  1, -1);   // diagonal down right
        }

        return res;
    }

    std::optional<Move> get_possible_move(const Chessboard& board,
                                          const PieceType& piece,
                                          const Color& color,
                                          const Position& from,
                                          const Position& to)
    {
        // Cannot move a piece to a cell that already
        // contains another piece of the same color.
        if (board[to] && board[to]->second == color)
            return std::nullopt;

        // Cannot move a piece to a cell if this move requires this
        // piece to go through a cell that already contains another
        // piece - regardless of its color (except for the Knight).
        if (piece != PieceType::KNIGHT)
            if (count_pieces_between(board, from, to) != 0)
                return std::nullopt;

        // Handle "en passant": if cell is free and is
        // board.en_passant_ it is a capture.
        if (piece == PieceType::PAWN && board.get_en_passant() == to)
            return Move(from, to, piece, true, false, false, false, true);

        // At this stage, board at position to is free
        // or occupied by the opposite color.
        bool capture = board[to].has_value();
        return Move(from, to, piece, capture, false, false, false, false);
    }

    void register_castling(const Chessboard& board,
                           std::vector<Move>&,
                           const Color& color)
    {
        // Find positions of king and the two rooks
        Position king = Position(File::E, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);
        Position left_rook = Position(File::A, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);
        Position right_rook = Position(File::H, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);

        // Check if allowed to do a king castling (RIGHT)
        if (board.get_king_castling(color)
            && count_pieces_between(board, king, right_rook) == 0)
        {
            // Determine new positions
            // Position new_king = Position(File::G, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);
            // Position new_right_rook = Position(File::F, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);

            // for (Position pos : get_positions_between(king, new_king))
            // {
            //      if king at pos would be in check => do not add move
            // }
        }

        // Check if allowed to do a queen castling (LEFT)
        if (board.get_queen_castling(color)
            && count_pieces_between(board, king, left_rook) == 0)
        {
            // Determine new positions
            // Position new_king = Position(File::C, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);
            // Position new_left_rook = Position(File::D, (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT);

            // for (Position pos : get_positions_between(king, new_king))
            // {
            //     if king at pos would be in check => do not add move
            // }
        }
    }

    void register_promotion(std::vector<Move>& moves,
                            const Position& from,
                            const Position& to,
                            const Color& color)
    {
        // Have the pawn reached the end?
        if (to.get_rank() != (color == Color::BLACK ? Rank::ONE : Rank::EIGHT))
            return;

        // Create the moves
        moves.emplace_back(from, to, PieceType::PAWN,
                           false, false, false, false, false,
                           PieceType::QUEEN);
        moves.emplace_back(from, to, PieceType::PAWN,
                           false, false, false, false, false,
                           PieceType::ROOK);
        moves.emplace_back(from, to, PieceType::PAWN,
                           false, false, false, false, false,
                           PieceType::BISHOP);
        moves.emplace_back(from, to, PieceType::PAWN,
                           false, false, false, false, false,
                           PieceType::KNIGHT);
        moves.emplace_back(from, to, PieceType::PAWN,
                           false, false, false, false, false,
                           PieceType::PAWN);
    }

    std::vector<Move> generate_moves(const Chessboard& board,
                                     const PieceType& piece)
    {
        std::vector<Move> res;
        Color color = Color::WHITE; // FIXME: color depends on board.white_turn_
        auto pieces_positions = get_pieces_positions(board, piece, color);

        // Generate regular moves
        for (Position from : pieces_positions)
        {
            // Step 1: Authorized (on the correct trajectory)
            auto authorized_pos = get_authorized_pos(piece, from);
            for (auto to : authorized_pos)
            {
                // Step 2: Possible (cell occupied, capture?)
                std::optional<Move> move = get_possible_move(board, piece,
                                                             color, from, to);

                if (move)
                    res.emplace_back(*move);

                // Handle promotion
                if (piece == PieceType::PAWN)
                    register_promotion(res, from, to, color);
            }

            // Handle castling moves
            if (piece == PieceType::KING)
                register_castling(board, res, color);
        }

        return res;
    }

    std::vector<Move> generate_pawn_moves(const Chessboard& board)
    {
        std::vector<Move> res;
        Color color = Color::WHITE; // FIXME: color depends on board.white_turn_
        PieceType piece = PieceType::PAWN;
        auto pieces_positions = get_pieces_positions(board, piece, color);

        for (Position from : pieces_positions)
        {
            // Pawn cannot capture a piece that is in front of it and
            // obviously cannot capture same color.
            std::optional<Position> to_forward = (color == Color::BLACK)
                ? from.move(0, -1)
                : from.move(0,  1);
            if (to_forward && !board[*to_forward])
                res.emplace_back(from, *to_forward, piece,
                                 false, false, false, false, false);

            std::optional<Position> to_forward_2 = (color == Color::BLACK)
                ? from.move(0, -2)
                : from.move(0,  2);
            bool first_move = (color == Color::BLACK)
                ? from.get_rank() == Rank::TWO
                : from.get_rank() == Rank::SEVEN;
            if (first_move && to_forward_2 && !board[*to_forward_2])
                res.emplace_back(from, *to_forward_2, piece,
                                 false, true, false, false, false);

            // Pawn can move to a cell diagonally in front of it on an adjacent
            // file if (and only if) the cell already contains a piece of the
            // other color on it. In this case it is also a capture.
            std::optional<Position> to_diag_left = (color == Color::BLACK)
                ? from.move(-1, -1)
                : from.move(-1,  1);

            std::optional<Position> to_diag_right = (color == Color::BLACK)
                ? from.move(1, -1)
                : from.move(1,  1);
            
            if (to_diag_left
                && board[*to_diag_left]
                && board[*to_diag_left]->second != color)
                res.emplace_back(from, *to_diag_left, piece,
                                 true, false, false, false, false);

            if (to_diag_right
                && board[*to_diag_right]
                && board[*to_diag_right]->second != color)
                res.emplace_back(from, *to_diag_right, piece,
                                 true, false, false, false, false);
        }

        return res;
    }

    std::vector<Move> generate_king_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::KING);
    }

    std::vector<Move> generate_bishop_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::BISHOP);
    }

    std::vector<Move> generate_rook_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::ROOK);
    }

    std::vector<Move> generate_queen_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::QUEEN);
    }

    std::vector<Move> generate_knight_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::KNIGHT);
    }

} // namespace rule