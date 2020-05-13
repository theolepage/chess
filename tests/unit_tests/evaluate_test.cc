#include "gtest/gtest.h"

#include "chess_engine/ai/evaluation.hh"

using namespace board;
using namespace ai;

TEST(Evaluate, EmptyBoard)
{
    Chessboard board = Chessboard("8/8/8/8/8/8/8/8");

    EXPECT_EQ(evaluate(board), 0);
}

TEST(Evaluate, WhiteAdvantage)
{
    Chessboard board = Chessboard("nP6/PP6/8/8/8/8/8/8");

    EXPECT_GT(evaluate(board), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
