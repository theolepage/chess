#pragma once

#include "fen-object.hh"

namespace perft_parser
{
    class PerftObject
    {
    public:
        PerftObject(const FenObject &f, int depth)
        {
            fen_ = f;
            depth_ = depth;
        }

        FenObject fen_get(void) const
        {
            return fen_;
        }

        int depth_get(void) const
        {
            return depth_;
        }

    private:
        FenObject fen_;
        int depth_;
    };
}