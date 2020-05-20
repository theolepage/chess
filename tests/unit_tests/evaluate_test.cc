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

// NOTE It assumes that we only consider material and piece square tables
TEST(MaterialAndPieceSquareTables, Pawn)
{
    Chessboard board = Chessboard("8/8/8/8/8/8/1p6/8");

    EXPECT_EQ(evaluate(board), 50 + 100);
}

// NOTE It assumes that we only consider material and piece square tables
TEST(MaterialAndPieceSquareTables, Queen)
{
    Chessboard board = Chessboard("8/8/8/q7/8/8/8/8");

    EXPECT_EQ(evaluate(board), 0 + 900);
}

// NOTE It assumes that we only consider material and piece square tables
TEST(MaterialAndPieceSquareTables, Rook)
{
    Chessboard board = Chessboard("8/8/8/8/8/8/7r/8");

    EXPECT_TRUE(false);

    EXPECT_EQ(evaluate(board), 5 + 500);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
