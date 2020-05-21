#include <optional>
#include <algorithm>
#include <vector>

#include "ai-mini.hh"
#include "evaluation.hh"
#include "chess_engine/board/entity/color.hh"

namespace ai
{
     using evalAndMove = std::pair<int16_t, std::optional<board::Move>>;

     static evalAndMove minimax(board::Chessboard& chessboard,
                                   const int16_t depth,
                                   int16_t alpha,
                                   int16_t beta,
                                   const bool isMaxPlayer)
     {
          const std::vector<board::Move> legal_moves =
                    chessboard.generate_legal_moves();

          bool is_check = chessboard.is_check();
          if (chessboard.is_draw(legal_moves, is_check))
               return evalAndMove(0, std::nullopt);
          if (chessboard.is_checkmate(legal_moves, is_check))
               return evalAndMove(isMaxPlayer ? INT16_MIN : INT16_MAX,
                                  std::nullopt);
          if (depth == 0)
               return evalAndMove(evaluate(chessboard), std::nullopt);

          if (isMaxPlayer)
          {
               int16_t bestValue = INT16_MIN;
               size_t bestIndex = 0;
               for (size_t i = 0; i < legal_moves.size(); i++)
               {
                    board::Chessboard chessboard_ = chessboard;
                    chessboard_.do_move(legal_moves[i]);
                    const int16_t eval = minimax(chessboard_, depth - 1,
                                                 alpha, beta,
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
               return evalAndMove(bestValue, legal_moves[bestIndex]);
          }
          // else
          int16_t bestValue = INT16_MAX;
          size_t bestIndex = 0;
          for (size_t i = 0; i < legal_moves.size(); i++)
          {
               board::Chessboard chessboard_ = chessboard;
               chessboard_.do_move(legal_moves[i]);
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
          return evalAndMove(bestValue, legal_moves[bestIndex]);
     }

     board::Move AiMini::search(board::Chessboard& chessboard) const
     {
          return minimax(chessboard, 3, INT16_MIN, INT16_MAX,
                         chessboard.get_white_turn()).second.value();
     }
}
