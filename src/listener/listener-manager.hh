#pragma once

#include "listener.hh"
#include "chess_engine/board/chessboard-interface.hh"
#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/position.hh"
#include "parsing/pgn_parser/pgn-move.hh"

namespace listener
{
    class ListenerManager : public board::ChessboardInterface
    {
    public:
        virtual opt_piece_t operator[](const board::Position& /*position*/) const;

        void play_ai(void);

        void play_pgn_moves(const std::vector<board::PgnMove> moves);

        void add_listener(void* dll, Listener* listener);

        void close_listeners(void);

    private:
        std::vector<Listener*> listeners_;
        board::Chessboard chessboard_;
        std::vector<void*> dlls_;

        void notify_move(board::Move move, opt_piece_t taken_piece) const;
        bool notify_board_state();
        bool do_move_and_notify(board::Move move);
    };

}