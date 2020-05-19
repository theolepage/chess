#include <cassert>

#include "bit-boards.hh"
#include "position.hh"
#include "color.hh"

namespace board
{
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>;
    using opt_pos_t = std::optional<Position>;

    opt_piece_t Board::operator[](const Position& pos) const
    {
        const int index = pos.get_index();
        const bool white = utils::is_bit_set(whites_, index);
        const bool black = utils::is_bit_set(blacks_, index);
        if (!white && !black)
            return std::nullopt;

        const Color piece_color = ((white) ? Color::WHITE : Color::BLACK);
        if (utils::is_bit_set(pawns_, index))
            return opt_piece_t(PieceType::PAWN, piece_color);
        if (utils::is_bit_set(rooks_, index))
            return opt_piece_t(PieceType::ROOK, piece_color);
        if (utils::is_bit_set(knights_, index))
            return opt_piece_t(PieceType::KNIGHT, piece_color);
        if (utils::is_bit_set(bishops_, index))
            return opt_piece_t(PieceType::BISHOP, piece_color);
        if (utils::is_bit_set(queens_, index))
            return opt_piece_t(PieceType::QUEEN, piece_color);
        if (utils::is_bit_set(kings_, index))
            return opt_piece_t(PieceType::KING, piece_color);

        assert(false);
        return std::nullopt;
    }

    void Chessboard::set_piece(const Position& pos,
                                const PieceType piecetype,
                                const Color color)
    {
        bitboard_t& piece_bitboard = get_bitboard(piecetype, color);

        const auto pos_rank_i = utils::utype(pos.get_rank());
        const auto pos_file_i = utils::utype(pos.get_file());

        piece_bitboard[pos_rank_i].set(pos_file_i);
    }

    void Chessboard::unset_piece(const Position& pos,
                                const PieceType piecetype,
                                const Color color)
    {
        bitboard_t& piece_bitboard = get_bitboard(piecetype, color);

        const auto pos_rank_i = utils::utype(pos.get_rank());
        const auto pos_file_i = utils::utype(pos.get_file());

        piece_bitboard[pos_rank_i].reset(pos_file_i);
    }

    void Chessboard::move_piece(const Position& start,
                                const Position& end,
                                const PieceType piecetype,
                                const Color color)
    {
        unset_piece(start, piecetype, color);
        set_piece(end, piecetype, color);
    }

    void Chessboard::change_piece_type(const Position& pos,
                                        const PieceType old_type,
                                        const PieceType new_type,
                                        const Color color)
    {
        unset_piece(pos, old_type, color);
        set_piece(pos, new_type, color);
    }

        void Chessboard::init_end_ranks(const PieceType piecetype, const File file)
    {
        constexpr Rank white_end_rank = Rank::ONE;
        constexpr Rank black_end_rank = Rank::EIGHT;

        set_piece(Position(file, white_end_rank), piecetype, Color::WHITE);
        set_piece(Position(file, black_end_rank), piecetype, Color::BLACK);
    }

    File symetric_file(const File file)
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

    void Chessboard::symetric_init_end_ranks(const PieceType piecetype,
                                             const File file)
    {
        init_end_ranks(piecetype, file);
        init_end_ranks(piecetype, symetric_file(file));
    }

}