#include "chessboard.hh"

#include "parsing/option_parser/option-parser.hh"
#include "rule.hh"
#include "piece-type.hh"
#include "color.hh"
#include "position.hh"
#include "move.hh"

#include <cassert>
#include <optional>
#include <sstream>

namespace board
{
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>;
    using opt_pos_t = std::optional<board::Position>;

    const Chessboard::bitboard_t& Chessboard::get_bitboard(PieceType piecetype, Color color) const
    {
        const auto piecetype_i = utils::utype(piecetype);

        return color == Color::WHITE ?
            white_bitboards_[piecetype_i] :
            black_bitboards_[piecetype_i];
    }

    // TODO can we do better than code duplication ?
    Chessboard::bitboard_t& Chessboard::get_bitboard(PieceType piecetype, Color color)
    {
        const auto piecetype_i = utils::utype(piecetype);

        return color == Color::WHITE ?
            white_bitboards_[piecetype_i] :
            black_bitboards_[piecetype_i];
    }

    size_t Chessboard::get_bitboard_count(const PieceType piecetype, const Color color) const
    {
        const bitboard_t& bitboard = get_bitboard(piecetype, color);

        size_t count = 0;

        for (const auto& line : bitboard)
            count += line.count();

        return count;
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

    void Chessboard::symetric_init_end_ranks(const PieceType piecetype, const File file)
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
            const auto file = static_cast<File>(file_i);
            set_piece(Position(file, Rank::TWO), PieceType::PAWN, Color::WHITE);
            set_piece(Position(file, Rank::SEVEN), PieceType::PAWN, Color::BLACK);
        }

        symetric_init_end_ranks(PieceType::ROOK, File::A);
        symetric_init_end_ranks(PieceType::KNIGHT, File::B);
        symetric_init_end_ranks(PieceType::BISHOP, File::C);
        init_end_ranks(PieceType::QUEEN, File::D);
        init_end_ranks(PieceType::KING, File::E);

        register_state();
    }

    Chessboard::Chessboard(const FenObject& fen)
    {
        white_turn_ = fen.side_to_move_to_get() == Color::WHITE;

        const auto castling_chars = fen.castling_get();
        const auto chars_begin = castling_chars.begin();
        const auto chars_end = castling_chars.end();

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
            const auto rank = static_cast<Rank>(rank_i);

            for (size_t file_i = 0; file_i < width; file_i++)
            {
                const auto file = static_cast<File>(file_i);

                const auto pos = Position(file, rank);
                const auto opt_side_piece = fen[pos];
                if (opt_side_piece.has_value())
                {
                    const auto side_piece = opt_side_piece.value();
                    set_piece(pos, side_piece.first, side_piece.second);
                }
            }
        }

        register_state();
    }

    char Chessboard::sidepiece_to_char(const PieceType piecetype, const Color color)
    {
        const char piece_char = piece_to_char(piecetype);

        return color == Color::WHITE ? piece_char : tolower(piece_char);
    }

    char Chessboard::sidepiece_to_char(const side_piece_t& sidepiece)
    {
        return sidepiece_to_char(sidepiece.first, sidepiece.second);
    }

    std::ostream& Chessboard::write_fen_rank(std::ostream& os, const Rank rank) const
    {
        unsigned short empty_cells_count = 0;

        for (size_t file_i = 0; file_i < width; file_i++)
        {
            const auto file = static_cast<File>(file_i);

            const opt_piece_t opt_piece = (*this)[Position(file, rank)];

            if (opt_piece.has_value())
            {
                if (empty_cells_count != 0)
                {
                    os << empty_cells_count;
                    empty_cells_count = 0;
                }

                os << sidepiece_to_char(opt_piece.value());
            }
            else
            {
                empty_cells_count++;
            }
        }

        if (empty_cells_count != 0)
            os << empty_cells_count;

        return os;
    }

    std::ostream& Chessboard::write_fen_board(std::ostream& os) const
    {
        constexpr auto last_rank = Rank::EIGHT;
        constexpr auto last_rank_i = utils::utype(last_rank);

        write_fen_rank(os, last_rank);

        for (int rank_i = last_rank_i - 1; rank_i >= 0; rank_i--)
            write_fen_rank(os << '/', static_cast<Rank>(rank_i));

        return os;
    }

    std::string Chessboard::to_fen_string(void) const
    {
        std::stringstream ss;

        write_fen_board(ss);

        return ss.str();
    }

    void Chessboard::register_state()
    {
        std::string current_state = to_fen_string();

        if (state_count_.find(current_state) == state_count_.end())
            state_count_[current_state] = 1;
        else
            state_count_[current_state]++;
    }

    std::vector<Move> Chessboard::generate_legal_moves(void)
    {
        std::vector<Move> legal_moves;

        const std::vector<Move> possible_moves = rule::generate_all_moves(*this);

        for (const Move& move : possible_moves)
            if (is_possible_move_legal(move))
                legal_moves.push_back(move);

        return legal_moves;
    }

    void Chessboard::register_double_pawn_push(const Move& move, const Color color)
    {
        const Position& start = move.start_get();
        const Position& end = move.end_get();

        const auto start_rank_i = utils::utype(start.get_rank());
        const auto en_passant_rank_i = start_rank_i + (color == Color::WHITE ? 1 : -1);
        const auto en_passant_rank = static_cast<Rank>(en_passant_rank_i);

        en_passant_ = opt_pos_t(Position(end.get_file(), en_passant_rank));
    }

    void Chessboard::forget_en_passant(void)
    {
        if (en_passant_.has_value())
            en_passant_ = std::nullopt;
    }

    void Chessboard::update_last_fifty_turn(const Move& move)
    {
        if (move.capture_get() || move.piece_get() == PieceType::PAWN)
            last_fifty_turn_ = 0;
        else
            if (!white_turn_)
                last_fifty_turn_++;
    }

    void Chessboard::eat_en_passant(const Move& move, const Color color)
    {
        const Position& end = move.end_get();

        const auto en_passant_rank_i = utils::utype(end.get_rank());
        const auto eaten_pawn_rank_i = en_passant_rank_i +
            (color == Color::WHITE ? -1 : 1);
        const auto eaten_pawn_rank = static_cast<Rank>(eaten_pawn_rank_i);

        const auto eaten_pawn_color = color == Color::WHITE ?
            Color::BLACK : Color::WHITE;

        unset_piece(Position(end.get_file(), eaten_pawn_rank),
                       PieceType::PAWN, eaten_pawn_color);
    }

    void Chessboard::move_castling_rook(const Move& move, const Color color)
    {
        const Position& end = move.end_get();

        const auto king_file_i = utils::utype(end.get_file());
        const auto king_rank = end.get_rank();

        const auto rook_start_file = move.king_castling_get() ? File::H : File::A;
        const auto rook_end_file_i = king_file_i + (move.king_castling_get() ? -1 : 1);
        const auto rook_end_file = static_cast<File>(rook_end_file_i);

        const auto rook_start = Position(rook_start_file, king_rank);
        const auto rook_end = Position(rook_end_file, king_rank);

        move_piece(rook_start, rook_end, PieceType::ROOK, color);
    }

    void Chessboard::update_white_castling_bools(const Move& move)
    {
        const Position queenside_rook_pos = Position(File::A, Rank::ONE);
        const Position kingside_rook_pos = Position(File::H, Rank::ONE);

        if (move.castling_get())
        {
            white_king_castling_ = false;
            white_queen_castling_ = false;
        }
        else
        {
            const auto move_piecetype = move.piece_get();

            if (move_piecetype == PieceType::KING)
            {
                // If the king is moved, no castling will ever be possible again
                white_king_castling_ = false;
                white_queen_castling_ = false;
            }
            else if (move_piecetype == PieceType::ROOK)
            {
                const auto rook_start = move.start_get();

                // If a rook is moved, it can only cancels one castling out of two
                if (rook_start == kingside_rook_pos)
                    white_king_castling_ = false;
                else if (rook_start == queenside_rook_pos)
                    white_queen_castling_ = false;
            }
        }
    }

    void Chessboard::update_black_castling_bools(const Move& move)
    {
        const Position queenside_rook_pos = Position(File::A, Rank::EIGHT);
        const Position kingside_rook_pos = Position(File::H, Rank::EIGHT);

        if (move.castling_get())
        {
            black_king_castling_ = false;
            black_queen_castling_ = false;
        }
        else
        {
            const auto move_piecetype = move.piece_get();

            if (move_piecetype == PieceType::KING)
            {
                // If the king is moved, no castling will ever be possible again
                black_king_castling_ = false;
                black_queen_castling_ = false;
            }
            else if (move_piecetype == PieceType::ROOK)
            {
                const auto rook_start = move.start_get();

                // If a rook is moved, it can only cancels one castling out of two
                if (rook_start == kingside_rook_pos)
                    black_king_castling_ = false;
                else if (rook_start == queenside_rook_pos)
                    black_queen_castling_ = false;
            }
        }
    }

    void Chessboard::update_castling_bools(const Move& move, const Color color)
    {
        if (color == Color::WHITE)
            update_white_castling_bools(move);
        else
            update_black_castling_bools(move);
    }

    unsigned Chessboard::get_point_value(const Color color) const
    {
        unsigned point_value = 0;

        for (const auto& piecetype : piecetype_array)
        {
            const auto piecetype_i = utils::utype(piecetype);
            point_value += piecetype_value[piecetype_i] * get_bitboard_count(piecetype, color);
        }

        return point_value;
    }

    // point value implementation
    int Chessboard::evaluate(void) const
    {
        return get_point_value(Color::WHITE) - get_point_value(Color::BLACK);
    }

    void Chessboard::do_move(const Move& move)
    {
        const Position& start = move.start_get();
        const Position& end = move.end_get();
        const Color color = (*this)[start].value().second;
        const PieceType piecetype = move.piece_get();

        // The piece that will be eaten if move is a capture
        const opt_piece_t opt_end_piece = (*this)[end];

        move_piece(start, end, piecetype, color);

        // NOTE if a move is a double pawn push, then it cannot be a capture
        if (move.double_pawn_push_get())
            register_double_pawn_push(move, color);
        else
        {
            forget_en_passant();
            update_last_fifty_turn(move);

            if (move.en_passant_get())
                eat_en_passant(move, color);
            else if (move.castling_get())
                move_castling_rook(move, color);
            else if (move.capture_get())
            {
                assert(opt_end_piece.has_value());
                side_piece_t eaten_piece = opt_end_piece.value();
                auto eaten_piece_type = eaten_piece.first;
                auto eaten_piece_color = eaten_piece.second;

                assert(eaten_piece_color == (color == Color::WHITE ? Color::BLACK : Color::WHITE));

                unset_piece(end, eaten_piece_type, eaten_piece_color);
            }

            if (move.promotion_get().has_value())
            {
                const auto new_piecetype = move.promotion_get().value();
                change_piece_type(end, PieceType::PAWN, new_piecetype, color);
            }
        }
        update_castling_bools(move, color);

        // If black played, then a turned passed
        if (!white_turn_)
            turn_++;

        white_turn_ = !white_turn_;

        register_state();
    }

    /*// In this function we place the rook correctly, since the king will be placed back thanks to do_move
    void Chessboard::handle_undo_castling(const Move& move)
    {
        if (move.queen_castling_get())
        {
            // The white turn variable has not yet been toggeled since it's done in do_move
            if (white_turn_) // The black was playing
            {
                unset_piece(move.start_get(), move.promotion_get().value(), ((white_turn_) ? Color::WHITE : Color::BLACK));
                set_piece(move.start_get(), PieceType::PAWN, ((white_turn_) ? Color::WHITE : Color::BLACK));
            }
        }
    }

    void Chessboard::undo_move(const Move& move, const option_parser::BoardState& state)
    {
        // Handle special case or just make move in reverse
        if (move.king_castling_get() || move.queen_castling_get())
        {
            handle_undo_castling(move);
        }

        const Move reversed = move.get_reverse();
        do_move(reversed);

        // Then restore any eaten piece that was at end position
        // We stored where the pawn was eaten so en passant is handled
        if (state.ate)
        {
            // The white was currently playing, a black piece was eaten
            set_piece(Position(state.eaten_x, state.eaten_y), static_cast<PieceType>(state.piece_type),
                        ((white_turn_) ? Color::BLACK : Color::WHITE));
        }

        // If there was a promotion we need to restore as a pawn
        if (move.promotion_get().has_value())
        {
            unset_piece(move.start_get(), move.promotion_get().value(), ((white_turn_) ? Color::WHITE : Color::BLACK));
            set_piece(move.start_get(), PieceType::PAWN, ((white_turn_) ? Color::WHITE : Color::BLACK));
        }


        // Now restore the state flags

        white_king_castling_ = state.white_king_castling;
        white_queen_castling_ = state.white_queen_castling;
        black_king_castling_ = state.black_king_castling;
        black_queen_castling_ = state.black_queen_castling;

        // Also need to restore en passant flag
        if (state.en_passant)
        {
            en_passant_ = Position(state.en_passant_y, state.en_passant_y);
        }
    }*/

    bool Chessboard::is_move_possible(const Move& move)
    {
        // Move is invalid if in start the piece is not there or bad color
        // Scope is soooo perf, swiftly
        {
            const auto opt_piece = (*this)[move.start_get()];
            if (!opt_piece.has_value())
            {
                return false;
            }
            else if (opt_piece.value().first != move.piece_get()
                    || opt_piece.value().second != ((white_turn_) ? Color::WHITE : Color::BLACK))
            {
                return false;
            }
        }

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

        const auto start = possible_piecetype_moves.begin();
        const auto end = possible_piecetype_moves.end();

        return std::find(start, end, move) != end;
    }

    bool Chessboard::is_possible_move_legal(const Move& move) const
    {
        // TODO try do undo
        Chessboard board_copy = *this;
        board_copy.do_move(move);
        board_copy.white_turn_ = !board_copy.white_turn_;
        return !board_copy.is_check();

        /*option_parser::BoardState state;
        fill_state(move, *this, state);

        do_move(move);
        white_turn_ = !white_turn_;
        bool no_checked = !is_check();
        white_turn_ = !white_turn_;
        undo_move(move, state);
        turn_ -= 2; // We called do_move 2 times

        return no_checked;*/
    }

    bool Chessboard::is_move_legal(const Move& move)
    {
        return is_move_possible(move) && is_possible_move_legal(move);
    }

    Position Chessboard::get_king_position(void) const
    {
        const auto king_color = white_turn_ ? Color::WHITE : Color::BLACK;
        const bitboard_t& king_bitboard = get_bitboard(PieceType::KING, king_color);

        size_t rank_i = 0;
        while (king_bitboard[rank_i].none())
            rank_i++;

        size_t file_i = 0;
        while (!king_bitboard[rank_i][file_i])
            file_i++;

        return Position(static_cast<File>(file_i), static_cast<Rank>(rank_i));
    }

    bool Chessboard::is_check(void)
    {
        const Position king_pos = get_king_position();

        // little hack to get the opponent turns
        white_turn_ = !white_turn_;
        const auto possible_moves = rule::generate_all_moves(*this);
        white_turn_ = !white_turn_;

        for (const Move& move : possible_moves)
            if (move.end_get() == king_pos)
                return true;

        return false;
    }

    bool Chessboard::is_pat(void)
    {
        return !is_check() && generate_legal_moves().empty();
    }

    bool Chessboard::is_checkmate(void)
    {
        return is_check() && generate_legal_moves().empty();
    }

    bool Chessboard::threefold_repetition()
    {
        std::string current_state = to_fen_string();

        assert(state_count_.find(current_state) != state_count_.end());

        return state_count_[current_state] >= 3;
    }

    bool Chessboard::is_draw(void)
    {
        return last_fifty_turn_ >= 50 || is_pat() || threefold_repetition();
    }

    bool Chessboard::get_white_turn(void) const
    {
        return white_turn_;
    }

    void Chessboard::set_white_turn(const bool state)
    {
        white_turn_ = state;
    }

    Chessboard::opt_pos_t Chessboard::get_en_passant(void) const
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

    void Chessboard::set_king_castling(const Color& color, const bool state)
    {
        if (color == Color::WHITE)
            white_king_castling_ = state;
        else if (color == Color::BLACK)
            black_king_castling_ = state;
    }

    void Chessboard::set_queen_castling(const Color& color, const bool state)
    {
        if (color == Color::WHITE)
            white_queen_castling_ = state;
        else if (color == Color::BLACK)
            black_queen_castling_ = state;
    }

    Chessboard::opt_piece_t Chessboard::operator[](const Position& pos) const
    {
        static constexpr auto colors = {Color::WHITE, Color::BLACK};
        const size_t rank_i = utils::utype(pos.get_rank());
        const size_t file_i = utils::utype(pos.get_file());

        for (const auto piecetype : piecetype_array)
        {
            for (const auto color : colors)
            {
                const bitboard_t& bitboard = get_bitboard(piecetype, color);

                if (bitboard[rank_i][file_i])
                    return std::make_pair(piecetype, color);
            }
        }

        return std::nullopt;
    }
}