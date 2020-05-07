#pragma once

#include <optional>
#include <vector>
#include <dlfcn.h>

#include "chess_engine/board/chessboard-interface.hh"
#include "chess_engine/board/color.hh"
#include "chess_engine/board/piece-type.hh"
#include "chess_engine/board/position.hh"
#include "listener.hh"
#include "chess_engine/board/chessboard.hh"

namespace listener
{

    class ListenerManager : public board::ChessboardInterface
    {
    public:
        virtual opt_piece_t operator[](const board::Position& /*position*/) const
        {
            // TODO need board
            return opt_piece_t();
        }

        void play_ai(void) const
        {
            // TODO need ai
        }

        void add_listener(void* dll, Listener* listener)
        {
            dlls_.emplace_back(dll);
            listeners_.emplace_back(listener);
        }

        void close_listeners(void)
        {
            for (void* dll : dlls_)
                dlclose(dll); // Do what on fail close
        }

    private:
        std::vector<Listener*> listeners_;
        board::Chessboard chessboard;
        std::vector<void*> dlls_;
    };

}