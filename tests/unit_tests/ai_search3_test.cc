#include "gtest/gtest.h"

#include "chess_engine/ai/ai-mini.hh"
#include "chess_engine/board/chessboard.hh"

using namespace board;

TEST(Check, minimax_first_move)
{
    ai::AiMini our_ai = ai::AiMini();
    Chessboard chessboard = Chessboard(parse_perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 0"));
    Move bestmove = our_ai.search(chessboard);
    EXPECT_EQ(PieceType::PAWN, bestmove.piece_get());
    EXPECT_EQ(Rank::THREE, bestmove.end_get().get_rank());
    EXPECT_EQ(File::E, bestmove.end_get().get_file());
}
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}