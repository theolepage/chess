#include <optional>
#include <vector>
#include <dlfcn.h>

#include "listener-manager.hh"
#include "chess_engine/board/color.hh"
#include "chess_engine/board/piece-type.hh"
#include "chess_engine/ai/uci.hh"

#define NOTIFY(FUNC) for (auto listener : listeners_) listener->FUNC
#define GET_COLOR() chessboard_.get_white_turn() ? board::Color::WHITE : board::Color::BLACK

namespace listener
{
    void ListenerManager::notify_move(board::Move move, opt_piece_t taken_piece) const
    {
        NOTIFY(on_piece_moved(move.piece_get(), move.start_get(),
                                    move.end_get()));
        if (move.capture_get()) //FIXME better way ?
            NOTIFY(on_piece_taken(taken_piece.value().first,
                                        move.end_get()));
        if (move.promotion_get().has_value())
            NOTIFY(on_piece_promoted(move.promotion_get().value(),
                                        move.end_get()));
        if (move.king_castling_get())
            NOTIFY(on_kingside_castling(GET_COLOR()));
        if (move.queen_castling_get())
            NOTIFY(on_queenside_castling(GET_COLOR()));
    }

    bool ListenerManager::notify_board_state()
    {
        if (chessboard_.is_check_mate())
        {
            NOTIFY(on_player_mat(GET_COLOR()));
            return false;
        }
        if (chessboard_.is_draw())
        {
            NOTIFY(on_draw());
            return false;
        }
        if (chessboard_.is_check())
            NOTIFY(on_player_check(GET_COLOR()));
        /*FIXME if (chessboard_.is_pat())
            NOTIFY(on_player_pat(GET_COLOR()));*/
        return true;
    }

    bool ListenerManager::do_move_and_notify(board::Move move)
    {
        if (chessboard_.is_move_legal(move))
        {
            opt_piece_t possibly_taken_piece = std::nullopt;
            if (move.capture_get()) //FIXME better way ?
                possibly_taken_piece.emplace(chessboard_[move.end_get()].value());
            chessboard_.do_move(move);
            notify_move(move, possibly_taken_piece);
        }
        else
        {
            NOTIFY(on_player_disqualified(GET_COLOR()));
            return false;
        }
        return notify_board_state();
    }

    void ListenerManager::play_ai(void)
    {
        // FIXME
        ai::init("NameOfOurBeautifulAi");

        /* TODO draft
        register_board(ai::get_board()) // FIXME do not call get board twice
        while( ? True ? ):
            board = ai::get_board
            if (board.is_checkmate())
                break;
            Move move = ai.search(board)
            ai::play_move(move_to_str(move))
            notify_move(move)

            notify_board_state(board) ???
        */
    }

    void ListenerManager::play_pgn_moves(const std::vector<board::PgnMove> moves)
    {
        if (moves.size() == 0)
            return;

        // load board
        chessboard_ = board::Chessboard(); // FIXME not needed, right ?
        NOTIFY(register_board(chessboard_));

        // first move
        board::Move move = moves[0].to_Move();
        if (!do_move_and_notify(move))
        {
            NOTIFY(on_game_finished());
            return;
        }

        // all other moves
        for (board::PgnMove pgnMove : moves)
        {
            move = pgnMove.to_Move(move); // FIXME by changing move in to_Move
            if (!do_move_and_notify(move))
            {
                NOTIFY(on_game_finished());
                break;
            }
        }
    }

    void ListenerManager::add_listener(void* dll, Listener* listener)
    {
        dlls_.emplace_back(dll);
        listeners_.emplace_back(listener);
    }

    void ListenerManager::close_listeners(void)
    {
        for (void* dll : dlls_)
            dlclose(dll); // Do what on fail close
    }
}