#include "chessboard.hh"

namespace board
{
    void Chessboard::init_end_ranks(PieceType piecetype, File file)
    {
        constexpr size_t white_end_rank_i = 0;
        constexpr size_t black_end_rank_i = width - 1;

        size_t bitboard_i = utils::utype(piecetype);
        const size_t file_i = utils::utype(file);

        white_bitboards_[bitboard_i][white_end_rank_i].set(file_i);
        black_bitboards_[bitboard_i][black_end_rank_i].set(file_i);
    }

    File symetric_file(File file)
    {
        switch (file)
        {
        case File::A:
            return File::H;
        case File::B:
            return File::G;
        case File::C:
            return File::F;
        case File::D:
            return File::E;
        case File::E:
            return File::D;
        case File::F:
            return File::C;
        case File::G:
            return File::B;
        case File::H:
            return File::A;
        default:
            throw std::invalid_argument("Unknown file");
        }
    }

    void Chessboard::symetric_init_end_ranks(PieceType piecetype, File file)
    {
        init_end_ranks(piecetype, file);
        init_end_ranks(piecetype, symetric_file(file));
    }

    Chessboard::Chessboard()
    {
        const size_t pawn_i = utils::utype(PieceType::PAWN);
        white_bitboards_[pawn_i][1].set();
        black_bitboards_[pawn_i][width - 2].set();

        symetric_init_end_ranks(PieceType::ROOK, File::A);
        symetric_init_end_ranks(PieceType::KNIGHT, File::B);
        symetric_init_end_ranks(PieceType::BISHOP, File::C);
        init_end_ranks(PieceType::QUEEN, File::D);
        init_end_ranks(PieceType::KING, File::E);
    }

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

    Chessboard::opt_piece_t Chessboard::operator[](const Position& pos) const
    {
        size_t rank_i = utils::utype(pos.rank_get());
        size_t file_i = utils::utype(pos.file_get());

        for (auto piecetype : piecetype_array)
        {
            const auto piecetype_i = utils::utype(piecetype);
            const bitboard_t white_piecetype_bitboard = white_bitboards_[piecetype_i];
            const bitboard_t black_piecetype_bitboard = black_bitboards_[piecetype_i];

            if (white_piecetype_bitboard[rank_i][file_i])
                return std::make_pair(piecetype, Color::WHITE);

            if (black_piecetype_bitboard[rank_i][file_i])
                return std::make_pair(piecetype, Color::BLACK);
        }

        return std::nullopt;
    }
}
