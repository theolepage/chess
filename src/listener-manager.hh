#pragma once

#include <optional>
#include <vector>
#include <dlfcn.h>

#include "chessboard-interface.hh"
#include "color.hh"
#include "piece-type.hh"
#include "position.hh"
#include "listener.hh"
#include "chessboard.hh"

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