#include "chessboard.hh"
#include "rule.hh"

namespace board
{
    Chessboard::bitboard_t& Chessboard::get_bitboard(PieceType piecetype, Color color)
    {
        const auto piecetype_i = utils::utype(piecetype);

        return color == Color::WHITE ?
            white_bitboards_[piecetype_i] :
            black_bitboards_[piecetype_i];
    }

    void Chessboard::set_position(const Position& pos, PieceType piecetype, Color color)
    {
        bitboard_t& piece_bitboard = get_bitboard(piecetype, color);

        const auto pos_rank_i = utils::utype(pos.get_rank());
        const auto pos_file_i = utils::utype(pos.get_file());

        piece_bitboard[pos_rank_i].set(pos_file_i);
    }

    void Chessboard::unset_position(const Position& pos, PieceType piecetype, Color color)
    {
        bitboard_t& piece_bitboard = get_bitboard(piecetype, color);

        const auto pos_rank_i = utils::utype(pos.get_rank());
        const auto pos_file_i = utils::utype(pos.get_file());

        piece_bitboard[pos_rank_i].reset(pos_file_i);
    }

    void Chessboard::init_end_ranks(PieceType piecetype, File file)
    {
        constexpr Rank white_end_rank = Rank::ONE;
        constexpr Rank black_end_rank = Rank::EIGHT;

        set_position(Position(file, white_end_rank), piecetype, Color::WHITE);
        set_position(Position(file, black_end_rank), piecetype, Color::BLACK);
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

        white_king_castling_ = true;
        white_queen_castling_ = true;
        black_king_castling_ = true;
        black_queen_castling_ = true;

        en_passant_ = std::nullopt;

        turn_ = 0;
        last_fifty_turn_ = 0;

        const size_t pawn_i = utils::utype(PieceType::PAWN);
        white_bitboards_[pawn_i][1].set();
        black_bitboards_[pawn_i][width - 2].set();

        for (size_t file_i = 0; file_i < width; file_i++)
        {
            auto file = static_cast<File>(file_i);
            set_position(Position(file, Rank::TWO), PieceType::PAWN, Color::WHITE);
            set_position(Position(file, Rank::SEVEN), PieceType::PAWN, Color::BLACK);
        }

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

    void Chessboard::do_move(const Move& move)
    {
        const Position& start = move.start_get();
        const Position& end = move.end_get();
        const Color color = (*this)[start].value().second;
        const PieceType piecetype = move.piece_get();

        unset_position(start, piecetype, color);
        set_position(end, piecetype, color);

        if (move.double_pawn_push_get())
        {
            // double pawn push handling
            const auto start_rank_i = utils::utype(start.get_rank());
            const auto en_passant_rank_i = start_rank_i + (color == Color::WHITE ? 1 : -1);
            const auto en_passant_rank = static_cast<Rank>(en_passant_rank_i);

            en_passant_ = opt_pos_t(Position(end.get_file(), en_passant_rank));
        }
        else
        {
            if (en_passant_.has_value())
                en_passant_ = std::nullopt;

            // erase the eaten pawn
            if (move.en_passant_get())
            {
                const auto en_passant_rank_i = utils::utype(end.get_rank());
                const auto eaten_pawn_rank_i = en_passant_rank_i +
                    (color == Color::WHITE ? -1 : 1);
                const auto eaten_pawn_rank = static_cast<Rank>(eaten_pawn_rank_i);

                const auto eaten_pawn_color = color == Color::WHITE ?
                    Color::BLACK : Color::WHITE;

                unset_position(Position(end.get_file(), eaten_pawn_rank),
                               PieceType::PAWN, eaten_pawn_color);
            }
        }

        white_turn_ = !white_turn_;
    }

    bool Chessboard::is_move_legal(const Move& move)
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
