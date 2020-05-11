#include <optional>
#include <algorithm>
#include <vector>

#include "ai-mini.hh"
#include "chess_engine/board/color.hh"

namespace ai
{
     using evalAndMove = std::pair<int16_t, std::optional<board::Move>>;

     static void log_minimax(board::Move move, int16_t depth, int16_t eval)
     {
          std::cout << "depth: " << depth << " | eval : " << eval << " | ";
          std::cout << move << std::endl;
     }

     static evalAndMove minimax(board::Chessboard chessboard,
                                               int16_t depth, int16_t alpha,
                                               int16_t beta, bool isMaxPlayer)
     {
          if (chessboard.is_draw())
               return evalAndMove(0, std::nullopt);
          if (chessboard.is_checkmate())
               return evalAndMove(isMaxPlayer ? INT16_MIN : INT16_MAX,
                                  std::nullopt);
          if (depth == 0)
               return evalAndMove(chessboard.evaluate(), std::nullopt);

          //TODO opti generate_legal_moves
          std::vector<board::Move> moves = chessboard.generate_legal_moves();

          if (isMaxPlayer)
          {
               int16_t bestValue = INT16_MIN;
               size_t bestIndex = 0;
               for (size_t i = 0; i < moves.size(); i++)
               {
                    board::Chessboard chessboard_ = chessboard;
                    chessboard_.do_move(moves[i]);
                    int16_t eval = minimax(chessboard_, depth - 1, alpha, beta,
                                           !isMaxPlayer).first;
                    log_minimax(moves[i], depth, eval);
                    if (eval > bestValue)
                    {
                         bestValue = bestValue;
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
               log_minimax(moves[i], depth, eval);
               if (eval < bestValue)
               {
                    bestValue = std::min(bestValue, eval);
                    bestIndex = i;
                    beta = std::min(alpha, eval);
                    if (beta <= alpha)
                         break;
               }
          }
          return evalAndMove(bestValue, moves[bestIndex]);
     }

     board::Move AiMini::search(board::Chessboard chessboard)
     {
          return minimax(chessboard, 4, INT16_MIN, INT16_MAX, true).second.value();
     }
}