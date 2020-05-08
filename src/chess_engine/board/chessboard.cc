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

    Chessboard::Chessboard(const FenObject& fen)
    {
        white_turn_ = fen.side_to_move_to_get() == Color::WHITE;

        auto castling_chars = fen.castling_get();
        auto chars_begin = castling_chars.begin();
        auto chars_end = castling_chars.end();

        white_king_castling_ = std::find(chars_begin, chars_end, 'K') != chars_end;
        white_queen_castling_ = std::find(chars_begin, chars_end, 'Q') != chars_end;
        black_king_castling_ = std::find(chars_begin, chars_end, 'k') != chars_end;
        black_queen_castling_ = std::find(chars_begin, chars_end, 'q') != chars_end;

        en_passant_ = fen.en_passant_target_get();

        // FIXME it should be an available data in the fen string
        turn_ = 0;
        last_fifty_turn_ = 0;

        for (size_t rank_i = 0; rank_i < width; rank_i++)
        {
            auto rank = static_cast<Rank>(rank_i);

            for (size_t file_i = 0; file_i < width; file_i++)
            {
                auto file = static_cast<File>(file_i);

                auto pos = Position(file, rank);
                auto opt_side_piece = fen[pos];
                if (opt_side_piece.has_value())
                {
                    auto side_piece = opt_side_piece.value();
                    set_position(pos, side_piece.first, side_piece.second);
                }
            }
        }
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        std::vector<Move> legal_moves;

        std::vector<Move> possible_moves = rule::generate_moves(*this);

        for (Move move : possible_moves)
            if (is_move_legal(move))
                legal_moves.push_back(move);

        return legal_moves;
    }

    void Chessboard::do_move(const Move& move)
    {
        const Position& start = move.start_get();
        const Position& end = move.end_get();
        const Color color = (*this)[start].value().second;
        const PieceType piecetype = move.piece_get();

        unset_position(start, piecetype, color);
        set_position(end, piecetype, color);

        // NOTE if a move is a double pawn push, then it cannot be a capture
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

            if (move.capture_get() || move.piece_get() == PieceType::PAWN)
                last_fifty_turn_ = 0;
            else
                last_fifty_turn_++;

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

        turn_++;
        white_turn_ = !white_turn_;
    }

    bool Chessboard::is_move_possible(const Move& move)
    {
        std::vector<Move> possible_piecetype_moves;

        switch (move.piece_get())
        {
            case PieceType::QUEEN:
                possible_piecetype_moves = rule::generate_queen_moves(*this);
                break;
            case PieceType::ROOK:
                possible_piecetype_moves = rule::generate_rook_moves(*this);
                break;
            case PieceType::BISHOP:
                possible_piecetype_moves = rule::generate_bishop_moves(*this);
                break;
            case PieceType::KNIGHT:
                possible_piecetype_moves = rule::generate_knight_moves(*this);
                break;
            case PieceType::PAWN:
                possible_piecetype_moves = rule::generate_pawn_moves(*this);
                break;
            case PieceType::KING:
                possible_piecetype_moves = rule::generate_king_moves(*this);
                break;
        };

        auto start = possible_piecetype_moves.begin();
        auto end = possible_piecetype_moves.end();

        return std::find(start, end, move) != end;
    }

    bool Chessboard::is_possible_move_legal(const Move& move)
    {
        Chessboard board_copy = *this;
        board_copy.do_move(move);

        return !board_copy.is_check();
    }

    bool Chessboard::is_move_legal(const Move& move)
    {
        return is_move_possible(move) && is_possible_move_legal(move);
    }

    Position Chessboard::get_king_position()
    {
        auto king_color = white_turn_ ? Color::WHITE : Color::BLACK;
        bitboard_t king_bitboard = get_bitboard(PieceType::KING, king_color);

        size_t rank_i = 0;
        while (king_bitboard[rank_i].none())
            rank_i++;

        size_t file_i = 0;
        while (!king_bitboard[rank_i][file_i])
            file_i++;

        return Position(static_cast<File>(file_i), static_cast<Rank>(rank_i));
    }

    bool Chessboard::is_check()
    {
        const Position king_pos = get_king_position();

        auto possible_moves = rule::generate_moves(*this);

        for (Move move : possible_moves)
            if (move.end_get() == king_pos)
                return true;

        return false;
    }

    bool Chessboard::is_check_mate()
    {
        return is_check() && generate_legal_moves().empty();
    }

    // TODO handle threefold repetition
    bool Chessboard::is_draw()
    {
        return last_fifty_turn_ >= 50 || generate_legal_moves().empty();
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
