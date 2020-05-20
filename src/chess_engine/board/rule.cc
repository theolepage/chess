#include <vector>

#include "rule.hh"
#include "chessboard.hh"
#include "entity/move.hh"
#include "utils/utype.hh"
#include "move-generation.hh"

using namespace board;

namespace rule
{
    static void register_pos(std::vector<Position>& v,
            const std::vector<std::optional<Position>>& positions)
    {
        for (const auto& pos : positions)
            if (pos)
                v.push_back(*pos);
    }

    static void register_pos_line(std::vector<Position>& v,
                                  const Chessboard& board,
                                  const Position& from,
                                  const int file,
                                  const int rank)
    {
        std::optional<Position> pos = from.translate(file, rank);
        while (pos)
        {
            v.push_back(*pos);
            if (board[*pos].has_value())
                break;

            pos = pos->translate(file, rank);
        }
    }

    bool is_king_checked(const Chessboard& board)
    {
        const Color opponent_color = !board.get_white_turn() ? Color::WHITE
                                                             : Color::BLACK;
        const Position king_pos = board.get_king_position();

        // Find threatening positions
        std::vector<Position> threatening_pawns;
        register_pos(threatening_pawns, {
            king_pos.translate(-1, opponent_color == Color::WHITE ? -1 : 1),
            king_pos.translate(1, opponent_color == Color::WHITE ? -1 : 1)
        });
        const auto threatening_knights = get_authorized_pos(board,
                                                            PieceType::KNIGHT,
                                                            king_pos);
        const auto threatening_kings = get_authorized_pos(board,
                                                          PieceType::KING,
                                                          king_pos);
        const auto threatening_lines = get_authorized_pos(board,
                                                          PieceType::ROOK,
                                                          king_pos);
        const auto threatening_diagonals = get_authorized_pos(board,
                                                             PieceType::BISHOP,
                                                             king_pos);

        for (const auto& pos : threatening_pawns)
            if (board(pos, PieceType::PAWN, opponent_color).has_value())
                return true;
        for (const auto& pos : threatening_knights)
            if (board(pos, PieceType::KNIGHT, opponent_color).has_value())
                return true;
        for (const auto& pos : threatening_kings)
            if (board(pos, PieceType::KING, opponent_color).has_value())
                return true;
        for (const auto& pos : threatening_lines)
            if (board(pos, PieceType::ROOK, opponent_color).has_value()
                || board(pos, PieceType::QUEEN, opponent_color).has_value())
                return true;
        for (const auto& pos : threatening_diagonals)
            if (board(pos, PieceType::BISHOP, opponent_color).has_value()
                || board(pos, PieceType::QUEEN, opponent_color).has_value())
                return true;
        return false;
    }

    std::vector<Position> get_pieces_positions(const Chessboard& board,
                                               const PieceType& piece,
                                               const Color& color)
    {
        std::vector<Position> res;
        for (int rank = 0; rank < 8; rank++)
        {
            for (int file = 0; file < 8; file++)
            {
                Position pos(file, rank);
                if (board[pos].has_value()
                    && board[pos].value().first == piece
                    && board[pos].value().second == color)
                {
                    res.emplace_back(pos);
                }
            }
        }
        return res;
    }

    std::vector<Position> get_positions_between(const Position& x,
                                                const Position& y)
    {
        std::vector<Position> res;
        const int x_file = utils::utype(x.get_file());
        const int x_rank = utils::utype(x.get_rank());
        const int y_file = utils::utype(y.get_file());
        const int y_rank = utils::utype(y.get_rank());

        // Determine shift_file and shift_rank
        int shift_file = y_file - x_file;
        if (shift_file > 0) shift_file = 1;
        else if (shift_file < 0) shift_file = -1;

        int shift_rank = y_rank - x_rank;
        if (shift_rank > 0) shift_rank = 1;
        else if (shift_rank < 0) shift_rank = -1;

        res.push_back(x);
        std::optional<Position> pos = x.translate(shift_file, shift_rank);
        while (pos && pos != y)
        {
            res.push_back(*pos);
            pos = pos->translate(shift_file, shift_rank);
        }
        res.push_back(y);

        return res;
    }

    bool have_pieces_between(const Chessboard& board,
                             const Position& x,
                             const Position& y)
    {
        bool res = false;
        auto positions = get_positions_between(x, y);
        for (size_t i = 1; !res && i < positions.size() - 1; i++)
            if (board[positions.at(i)].has_value())
                res = true;
        return res;
    }

    std::vector<Position> get_authorized_pos(const Chessboard& board,
                                             const PieceType& piece,
                                             const Position& from)
    {
        std::vector<Position> res;

        if (piece == PieceType::KING)
        {
            register_pos(res, {
                from.translate(-1,  1), from.translate(0,  1), from.translate(1,  1), //top
                from.translate(-1,  0), from.translate(1,  0),                   //center
                from.translate(-1, -1), from.translate(0, -1), from.translate(1, -1)  //bottom
            });
        }
        if (piece == PieceType::KNIGHT)
        {
            register_pos(res, {
                from.translate(-1,  2), from.translate(-2,  1),   // top left
                from.translate(1,  2), from.translate(2,  1),   // top right
                from.translate(-1, -2), from.translate(-2, -1),   // bottom left
                from.translate(1, -2), from.translate(2, -1)    // bottom right
            });
        }
        if (piece == PieceType::ROOK || piece == PieceType::QUEEN)
        {
            register_pos_line(res, board, from, -1,  0); // line left
            register_pos_line(res, board, from,  1,  0); // line right
            register_pos_line(res, board, from,  0,  1); // line up
            register_pos_line(res, board, from,  0, -1); // line down
        }
        if (piece == PieceType::BISHOP || piece == PieceType::QUEEN)
        {
            register_pos_line(res, board, from, -1,  1); // diagonal up left
            register_pos_line(res, board, from,  1,  1); // diagonal up right
            register_pos_line(res, board, from, -1, -1); // diagonal down left
            register_pos_line(res, board, from,  1, -1); // diagonal down right
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
        auto piece_at_dest = board[to];
        if (piece_at_dest && piece_at_dest->second == color)
            return std::nullopt;

        // Cannot move a piece to a cell if this move requires this
        // piece to go through a cell that already contains another
        // piece - regardless of its color (except for the Knight).
        // Already handled in get_authorized_pos()

        // Handle "en passant": if cell is free and is
        // board.en_passant_ it is a capture.
        if (piece == PieceType::PAWN && board.get_en_passant() == to)
            return Move(from, to, piece, true, false, false, false, true);

        // At this stage, board at position to is free
        // or occupied by the opposite color.
        bool capture = piece_at_dest.has_value();
        return Move(from, to, piece, capture, false, false, false, false);
    }

    std::optional<Move> register_castling(const Chessboard& board,
                                          const Color& color,
                                          bool king_castling)
    {
        // Find positions of king and rook
        const Rank rank = (color == Color::WHITE) ? Rank::ONE : Rank::EIGHT;
        const Position king = Position(File::E, rank);
        const Position rook = Position(king_castling ? File::H
                                                     : File::A, rank);
        const Position new_king = Position(king_castling ? File::G
                                                         : File::C, rank);
        // Position new_rook = Position(king_castling ? File::F
        //                                            : File::D, rank);

        // Check if allowed to do a castling
        if (have_pieces_between(board, king, rook)
            || (king_castling && !board.get_king_castling(color))
            || (!king_castling && !board.get_queen_castling(color)))
            return std::nullopt;

        // Check if king would be in check for each pos between
        // king and new_king
        bool not_in_check = true;
        const auto temp_positions = get_positions_between(king, new_king);
        Chessboard board_copy = board;
        Position prev_step = king;
        for (size_t i = 0; i < temp_positions.size(); i++)
        {
            const Position step = temp_positions.at(i);
            board_copy.get_board().move_piece(prev_step, step, PieceType::KING, color);
            if (board_copy.is_check())
                not_in_check = false;
            prev_step = step;
        }

        std::optional<Move> res = std::nullopt;
        if (not_in_check)
        {
            if (king_castling)
                res = Move(king, new_king, PieceType::KING,
                           false, false, false, true, false);
            else
                res = Move(king, new_king, PieceType::KING,
                           false, false, true, false, false);
        }
        return res;
    }

    bool register_promotion(std::vector<Move>& moves,
                            const Position& from,
                            const Position& to,
                            const Color& color,
                            bool capture)
    {
        // Have the pawn reached the end?
        if (to.get_rank() != (color == Color::BLACK ? Rank::ONE
                                                    : Rank::EIGHT))
            return false;

        // Create the moves
        moves.emplace_back(from, to, PieceType::PAWN,
                           capture, false, false, false, false,
                           PieceType::QUEEN);
        moves.emplace_back(from, to, PieceType::PAWN,
                           capture, false, false, false, false,
                           PieceType::ROOK);
        moves.emplace_back(from, to, PieceType::PAWN,
                           capture, false, false, false, false,
                           PieceType::BISHOP);
        moves.emplace_back(from, to, PieceType::PAWN,
                           capture, false, false, false, false,
                           PieceType::KNIGHT);
        return true;
    }

    std::vector<Move> generate_moves(const Chessboard& board,
                                     const PieceType& piece)
    {
        std::vector<Move> res;
        const Color color = board.get_white_turn() ? Color::WHITE
                                                   : Color::BLACK;
        const auto pieces_positions = get_pieces_positions(board, piece,
                                                           color);

        // Generate regular moves
        for (const Position& from : pieces_positions)
        {
            // Step 1: Authorized (on the correct trajectory)
            const auto authorized_pos = get_authorized_pos(board, piece, from);
            for (const auto& to : authorized_pos)
            {
                // Step 2: Possible (cell occupied, capture?)
                const auto move = get_possible_move(board, piece, color,
                                                    from, to);
                if (move)
                    res.push_back(*move);
            }

            // Handle castling moves
            if (piece == PieceType::KING)
            {
                const auto king_castling = register_castling(board, color,
                                                             true);
                const auto queen_castling = register_castling(board, color,
                                                              false);
                if (king_castling)
                    res.push_back(*king_castling);
                if (queen_castling)
                    res.push_back(*queen_castling);
            }
        }

        return res;
    }

    std::vector<Move> generate_pawn_moves(const Chessboard& board)
    {
        std::vector<Move> res;
        const Color color = board.get_white_turn() ? Color::WHITE
                                                   : Color::BLACK;
        const PieceType piece = PieceType::PAWN;
        const auto pieces_positions = get_pieces_positions(board, piece,
                                                           color);

        for (Position from : pieces_positions)
        {
            // Pawn cannot capture a piece that is in front of it and
            // obviously cannot capture same color.
            const std::optional<Position> to_forward = (color == Color::BLACK)
                ? from.translate(0, -1)
                : from.translate(0,  1);
            if (to_forward && !board[*to_forward])
            {
                if (!register_promotion(res, from, *to_forward, color, false))
                    res.emplace_back(from, *to_forward, piece,
                                     false, false, false, false, false);

                const std::optional<Position> to_forward_2 =
                        (color == Color::BLACK)
                        ? from.translate(0, -2)
                        : from.translate(0,  2);
                const bool first_move = (color == Color::BLACK)
                        ? from.get_rank() == Rank::SEVEN
                        : from.get_rank() == Rank::TWO;
                if (first_move && to_forward_2 && !board[*to_forward_2])
                    res.emplace_back(from, *to_forward_2, piece,
                                    false, true, false, false, false);
            }

            // Pawn can move to a cell diagonally in front of it on an adjacent
            // file if (and only if) the cell already contains a piece of the
            // other color on it. In this case it is also a capture.
            const std::optional<Position> to_diag_left =
                    (color == Color::BLACK)
                    ? from.translate(-1, -1)
                    : from.translate(-1,  1);
            const std::optional<Position> to_diag_right =
                    (color == Color::BLACK)
                    ? from.translate(1, -1)
                    : from.translate(1,  1);

            if (to_diag_left)
            {
                bool capture = board[*to_diag_left]
                    && board[*to_diag_left]->second != color;
                bool en_passant = board.get_en_passant().has_value()
                    && board.get_en_passant() == *to_diag_left;
                if (capture || en_passant)
                    if (!register_promotion(res, from, *to_diag_left, color,
                                            true))
                        res.emplace_back(from, *to_diag_left, piece,
                                        true, false, false, false, en_passant);
            }

            if (to_diag_right)
            {
                bool capture = board[*to_diag_right]
                    && board[*to_diag_right]->second != color;
                bool en_passant = board.get_en_passant().has_value()
                    && board.get_en_passant() == *to_diag_right;

                if (capture || en_passant)
                    if (!register_promotion(res, from, *to_diag_right, color,
                                            true))
                        res.emplace_back(from, *to_diag_right, piece,
                                        true, false, false, false, en_passant);
            }
        }

        return res;
    }

    std::vector<Move> generate_king_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::KING);
    }

    std::vector<Move> generate_bishop_moves(const Chessboard& board)
    {
        return move_generation::generate_bishop_moves(board);
        // return generate_moves(board, PieceType::BISHOP);
    }

    std::vector<Move> generate_rook_moves(const Chessboard& board)
    {
        return move_generation::generate_rook_moves(board);
        // return generate_moves(board, PieceType::ROOK);
    }

    std::vector<Move> generate_queen_moves(const Chessboard& board)
    {
        return move_generation::generate_queen_moves(board);
        // return generate_moves(board, PieceType::QUEEN);
    }

    std::vector<Move> generate_knight_moves(const Chessboard& board)
    {
        return generate_moves(board, PieceType::KNIGHT);
    }

    std::vector<Move> generate_all_moves(const Chessboard& board)
    {
        std::vector<Move> moves;

        const auto pawn_moves = generate_pawn_moves(board);
        const auto king_moves = generate_king_moves(board);
        const auto bishop_moves = generate_bishop_moves(board);
        const auto rook_moves = generate_rook_moves(board);
        const auto queen_moves = generate_queen_moves(board);
        const auto knight_moves = generate_knight_moves(board);

        moves.insert(moves.end(), pawn_moves.begin(), pawn_moves.end());
        moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
        moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
        moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
        moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
        moves.insert(moves.end(), king_moves.begin(), king_moves.end());

        return moves;
    }
} // namespace rule