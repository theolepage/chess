#pragma once

#include <optional>

#include "chess_engine/board/move.hh"
#include "chess_engine/board/chessboard.hh"

namespace ai
{
     class AiMini final
     {
     public:
          std::optional<board::Move> search(board::Chessboard& chessboard) const;
     };
}
