#pragma once

#include "chess_engine/board/move.hh"
#include "chess_engine/board/chessboard.hh"

namespace ai
{
     class AiMini final
     {
     public:
          board::Move search(board::Chessboard chessboard);

          int evaluate(board::Chessboard);
     };
}