#include "gtest/gtest.h"

#include "chess_engine/ai/ai-mini.hh"
#include "chess_engine/board/chessboard.hh"

using namespace board;

TEST(Check, minimax_search_simple_checkmat)
{
    ai::AiMini our_ai = ai::AiMini();
    Chessboard chessboard = Chessboard(parse_perft("6k1/8/4PPPP/8/8/8/8/1K6 w - - 0 0 0"));
    Move bestmove = our_ai.search(chessboard);
    EXPECT_EQ(PieceType::PAWN, bestmove.get_piece());
    EXPECT_EQ(Rank::SEVEN, bestmove.get_end().get_rank());
    EXPECT_EQ(File::E, bestmove.get_end().get_file());
}

TEST(Check, minimax_search_simple_checkmat2)
{
    ai::AiMini our_ai = ai::AiMini();
    Chessboard chessboard = Chessboard(parse_perft("4r2k/6pp/7N/3Q4/8/8/8/6K1 w - - 0 1 0"));
    // Chessboard chessboard = Chessboard(parse_perft("3qr2k/pbpp2pp/1p5N/3Q2b1/2P1P3/P7/1PP2PPP/R4RK1 w - - 0 1 0"));
    Move bestmove = our_ai.search(chessboard);
    EXPECT_EQ(PieceType::QUEEN, bestmove.get_piece());
    EXPECT_EQ(Rank::EIGHT, bestmove.get_end().get_rank());
    EXPECT_EQ(File::G, bestmove.get_end().get_file());
}

/*
TEST(Check, minimax_search_simple_checkmat3)
{
    ai::AiMini our_ai = ai::AiMini();
    Chessboard chessboard = Chessboard(parse_perft("r1bq2k1/ppp2r1p/2np1pNQ/2bNpp2/2B1P3/3P4/PPP2PPP/R3K2R w KQ - 0 1 0"));
    Move bestmove = our_ai.search(chessboard);
    EXPECT_EQ(PieceType::KNIGHT, bestmove.piece_get());
    EXPECT_EQ(Rank::SIX, bestmove.end_get().get_rank());
    EXPECT_EQ(File::F, bestmove.end_get().get_file());
}*/

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}