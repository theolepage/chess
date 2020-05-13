#include "gtest/gtest.h"

#include "chess_engine/ai/evaluation.hh"

using namespace board;
using namespace ai;

TEST(Evaluate, Equality)
{
    Chessboard board = Chessboard("1b2k3/5q1n/3p4/8/1K5P/3P4/2Q2R2/8");

    EXPECT_EQ(evaluate(board), 0);
}

TEST(Evaluate, WhiteAdvantage)
{
    Chessboard board = Chessboard("1b2k3/5p1n/3p4/8/1K5P/3P4/2Q2R2/8");

    const auto score = 8; //white is winning

    EXPECT_EQ(evaluate(board), score);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
