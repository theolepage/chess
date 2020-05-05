#include "chessboard-interface.hh"

#include <vector>

#include "chessboard.hh"
#include "move.hh"

namespace rule
{
    
    std::vector<board::Move> generate_pawn_moves(board::Chessboard board);
    std::vector<board::Move> generate_king_moves(board::Chessboard board);
    std::vector<board::Move> generate_bishop_moves(board::Chessboard board);
    std::vector<board::Move> generate_rook_moves(board::Chessboard board);
    std::vector<board::Move> generate_queen_moves(board::Chessboard board);
    std::vector<board::Move> generate_knight_moves(board::Chessboard board);

} // namespace rule