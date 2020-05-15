#include "ai-launcher.hh"
#include "chess_engine/ai/ai-mini.hh"
#include "chess_engine/ai/uci.hh"
#include "parsing/pgn_parser/ebnf-parser.hh"

namespace ai
{
    void play_ai(void)
    {
        uci::init("bLiPbLoP");
        AiMini ai = AiMini();
        board::Chessboard chessboard = board::Chessboard();
        while(true)
        {
            pgn_parser::add_move_to_board(chessboard, uci::get_board());
            board::Move move = ai.search(chessboard);
            uci::play_move(pgn_parser::move_to_string(move));
            chessboard.do_move(move);
        }
    }
}