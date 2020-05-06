// To-Do: Implement get_pieces()
// To-Do: Handle castling and "en passant", check and advanced rules

#include <vector>

#include "chessboard.hh"
#include "move.hh"

using namespace board;

namespace rule
{
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

    static std::vector<Position> get_authorized_pos(const PieceType& piece,
                                                    const Position& from)
    {
        std::vector<Position> res;

        if (piece == PieceType::KING)
        {
            register_pos(res, {
                from.move(-1, -1), from.move(0, -1), from.move(1, -1), // top
                from.move(-1,  0), from.move(1,  0),                   // center
                from.move(-1,  1), from.move(0,  1), from.move(1,  1)  // bottom
            });
        }
        else if (piece == PieceType::KNIGHT)
        {
            register_pos(res, {
                from.move(-1, -2), from.move(-2, -1),   // top left
                from.move( 1, -2), from.move( 2, -1),   // top right
                from.move(-1,  2), from.move(-2,  1),   // bottom left
                from.move( 1,  2), from.move( 2,  1)    // bottom right
            });
        }
        else if (piece == PieceType::ROOK || piece == PieceType::QUEEN)
        {
            register_pos_line(res, from, -1,  0);   // line left
            register_pos_line(res, from,  1,  0);   // line right
            register_pos_line(res, from,  0, -1);   // line up
            register_pos_line(res, from,  0,  1);   // line down
        }
        else if (piece == PieceType::BISHOP || piece == PieceType::QUEEN)
        {
            register_pos_line(res, from, -1, -1);   // diagonal up left
            register_pos_line(res, from,  1, -1);   // diagonal up right
            register_pos_line(res, from, -1,  1);   // diagonal down left
            register_pos_line(res, from,  1,  1);   // diagonal down right
        }

        return res;
    }

    static std::pair<bool, bool> is_possible_move(const Chessboard& board,
                                                  const PieceType& piece,
                                                  const Color& color,
                                                  const Position& from,
                                                  const Position& to)
    {
        // Cannot move a piece to a cell that already
        // contains another piece of the same color.
        if (board[to] && board[to]->second == color)
            return std::make_pair<>(false, false);

        // Cannot move a piece to a cell if this move requires this
        // piece to go through a cell that already contains another
        // piece - regardless of its color (except for the Knight).
        if (piece != PieceType::KNIGHT)
        {
            auto pieces_traversed = board.get_pieces(from, to, Axis::VERTICAL);
            if (!pieces_traversed.empty())
                return std::make_pair<>(false, false);
        }
        
        // At this stage, board at position to is free
        // or occupied by the opposite color.
        bool capture = board[to].has_value();
        return std::make_pair<>(true, capture);
    }

    static bool is_legal_move()
    {
        return true;
    }

    static std::vector<Move> generate_moves(const Chessboard& board,
                                            const PieceType& piece_type)
    {
        std::vector<Move> res;
        auto pieces = board.get_pieces(piece_type);

        for (auto piece : pieces)
        {
            Position from = piece.first;
            Color color = piece.second;

            // Step 1: Authorized (on the correct trajectory)
            auto authorized_pos = get_authorized_pos(piece_type, from);
            for (auto to : authorized_pos)
            {
                // Step 2: Possible (cell occupied, capture?)
                auto possible = is_possible_move(board, piece_type,
                                                 color, from, to);

                // Step 3: Legal (check)
                auto legal = is_legal_move();

                if (!possible.first || !legal)
                    continue;
                
                res.emplace_back(from, to, piece_type,
                                 possible.second, false, false, false, false);
            }
        }

        return res;
    }

    std::vector<Move> generate_pawn_moves(const Chessboard& board)
    {
        std::vector<Move> res;
        auto pieces = board.get_pieces(PieceType::PAWN);

        for (auto p : pieces)
        {
            Position from = p.first;
            Color color = p.second;

            // Pawn cannot capture a piece that is in front of it and
            // obviously cannot capture same color.
            std::optional<Position> to_forward = (color == Color::BLACK)
                ? from.move(0,  1)
                : from.move(0, -1);
            if (to_forward && !board[*to_forward])
                res.emplace_back(from, *to_forward, PieceType::PAWN,
                                 false, false, false, false, false);

            std::optional<Position> to_forward_2 = (color == Color::BLACK)
                ? from.move(0,  2)
                : from.move(0, -2);
            bool first_move = from.rank_get() == Rank::TWO || from.rank_get() == Rank::SEVEN;
            if (first_move && to_forward_2 && !board[*to_forward_2])
                res.emplace_back(from, *to_forward_2, PieceType::PAWN,
                                 false, false, false, false, false);

            // Pawn can move to a cell diagonally in front of it on an adjacent
            // file if (and only if) the cell already contains a piece of the
            // other color on it. In this case it is also a capture.
            std::optional<Position> to_diag_left = (color == Color::BLACK)
                ? from.move(-1,  1)
                : from.move(-1, -1);

            std::optional<Position> to_diag_right = (color == Color::BLACK)
                ? from.move(1,  1)
                : from.move(1, -1);
            
            if (to_diag_left
                && board[*to_diag_left]
                && board[*to_diag_left]->second != color)
                res.emplace_back(from, *to_diag_left, PieceType::PAWN,
                                 true, false, false, false, false);

            if (to_diag_right
                && board[*to_diag_right]
                && board[*to_diag_right]->second != color)
                res.emplace_back(from, *to_diag_right, PieceType::PAWN,
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