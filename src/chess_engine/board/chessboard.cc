#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/rule.hh"
#include "utils/utype.hh"

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
        white_turn_ = true;

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

    // FIXME only works with simple moves (move to an empty cell)
    void Chessboard::do_move(const Move& move)
    {
        const Position& move_start = move.start_get();
        const Position& move_end = move.end_get();

        Color moved_piece_color = (*this)[move_start].value().second;
        const auto piecetype_i = utils::utype(move.piece_get());

        bitboard_t& piece_bitboard = moved_piece_color == Color::WHITE ?
            white_bitboards_[piecetype_i] :
            black_bitboards_[piecetype_i];

        const auto start_rank_i = utils::utype(move_start.get_rank());
        const auto start_file_i = utils::utype(move_start.get_file());
        const auto end_rank_i = utils::utype(move_end.get_rank());
        const auto end_file_i = utils::utype(move_end.get_file());

        piece_bitboard[start_rank_i].reset(start_file_i);
        piece_bitboard[end_rank_i].set(end_file_i);

        white_turn_ = !white_turn_;
    }

    bool Chessboard::is_move_legal(const Move& move) const
    {
        std::vector<Move> legal_moves;

        switch (move.piece_get())
        {
            case PieceType::QUEEN:
                legal_moves = rule::generate_queen_moves(*this);
                break;
            case PieceType::ROOK:
                legal_moves = rule::generate_rook_moves(*this);
                break;
            case PieceType::BISHOP:
                legal_moves = rule::generate_bishop_moves(*this);
                break;
            case PieceType::KNIGHT:
                legal_moves = rule::generate_knight_moves(*this);
                break;
            case PieceType::PAWN:
                legal_moves = rule::generate_pawn_moves(*this);
                break;
            case PieceType::KING:
                legal_moves = rule::generate_king_moves(*this);
                break;
        };

        return std::find(legal_moves.begin(), legal_moves.end(), move) !=
            legal_moves.end();
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

    bool Chessboard::get_white_turn() const
    {
        return white_turn_;
    }

    Chessboard::opt_pos_t Chessboard::get_en_passant() const
    {
        return en_passant_;
    }

    bool Chessboard::get_king_castling(const Color& color) const
    {
        return color == Color::WHITE ?
            white_king_castling_:
            black_king_castling_;
    }

    bool Chessboard::get_queen_castling(const Color& color) const
    {
        return color == Color::WHITE ?
            white_queen_castling_:
            black_queen_castling_;
    }

    Chessboard::opt_piece_t Chessboard::operator[](const Position& pos) const
    {
        size_t rank_i = utils::utype(pos.get_rank());
        size_t file_i = utils::utype(pos.get_file());

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
