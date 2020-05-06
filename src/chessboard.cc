#include "chessboard.hh"

namespace board
{
    std::vector<Move> Chessboard::generate_legal_moves()
    {
        // FIXME
        return std::vector<Move>();
    }

    void Chessboard::do_move(const Move&)
    {
        // FIXME
    }

    bool Chessboard::is_move_legal(const Move&)
    {
        // FIXME
        return false;
    }

    bool Chessboard::is_check()
    {
        // FIXME
        return false;
    }

    bool Chessboard::is_check_mate()
    {
        // FIXME
        return false;
    }

    bool Chessboard::is_draw()
    {
        // FIXME
        return false;
    }

    Chessboard::opt_piece_t Chessboard::operator[](const Position&) const
    {
        // FIXME
        return std::nullopt;
    }
}
