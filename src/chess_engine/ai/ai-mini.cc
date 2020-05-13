#include <optional>
#include <algorithm>
#include <vector>

#include "ai-mini.hh"
#include "chess_engine/board/color.hh"

namespace ai
{
     using evalAndMove = std::pair<int16_t, std::optional<board::Move>>;

     static evalAndMove minimax(board::Chessboard& chessboard,
                                   const int16_t depth,
                                   int16_t alpha,
                                   int16_t beta,
                                   const bool isMaxPlayer)
     {
          if (chessboard.is_draw())
               return evalAndMove(0, std::nullopt);
          if (chessboard.is_checkmate())
               return evalAndMove(isMaxPlayer ? INT16_MIN : INT16_MAX,
                                  std::nullopt);
          if (depth == 0)
               return evalAndMove(chessboard.evaluate(), std::nullopt);

          //TODO opti generate_legal_moves (order ?)
          std::vector<board::Move> moves = chessboard.generate_legal_moves();

          if (isMaxPlayer)
          {
               int16_t bestValue = INT16_MIN;
               size_t bestIndex = 0;
               for (size_t i = 0; i < moves.size(); i++)
               {
                    board::Chessboard chessboard_ = chessboard;
                    chessboard_.do_move(moves[i]);
                    const int16_t eval = minimax(chessboard_, depth - 1, alpha, beta,
                                           !isMaxPlayer).first;
                    if (eval > bestValue)
                    {
                         bestValue = eval;
                         bestIndex = i;
                         alpha = std::max(alpha, eval);
                         if (beta <= alpha)
                              break;
                    }
               }
               return evalAndMove(bestValue, moves[bestIndex]);
          }
          // else
          int16_t bestValue = INT16_MAX;
          size_t bestIndex = 0;
          for (size_t i = 0; i < moves.size(); i++)
          {
               board::Chessboard chessboard_ = chessboard;
               chessboard_.do_move(moves[i]);
               int16_t eval = minimax(chessboard_, depth - 1, alpha, beta,
                                        !isMaxPlayer).first;
               if (eval < bestValue)
               {
                    bestValue = eval;
                    bestIndex = i;
                    beta = std::min(beta, eval);
                    if (beta <= alpha)
                         break;
               }
          }
          return evalAndMove(bestValue, moves[bestIndex]);
     }

     board::Move AiMini::search(board::Chessboard& chessboard) const
     {
          return minimax(chessboard, 3, INT16_MIN, INT16_MAX,
                         chessboard.get_white_turn()).second.value();
     }
}