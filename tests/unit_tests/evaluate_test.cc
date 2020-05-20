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

TEST(EvaluateMaterial, SimplePawn)
{
    Chessboard white_board = Chessboard("8/1P6/8/8/8/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/8/8/8/1p6/8");

    const int abs_board_value = 100;

    EXPECT_EQ(evaluate_material(white_board), abs_board_value);
    EXPECT_EQ(evaluate_material(black_board), -abs_board_value);
}

TEST(EvaluateMaterial, SimpleQueen)
{
    Chessboard white_board = Chessboard("8/8/8/8/Q7/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/q7/8/8/8/8");

    const int abs_board_value = 900;

    EXPECT_EQ(evaluate_material(white_board), abs_board_value);
    EXPECT_EQ(evaluate_material(black_board), -abs_board_value);
}

TEST(EvaluateMaterial, SimpleRook)
{
    Chessboard white_board = Chessboard("8/7R/8/8/8/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/8/8/8/7r/8");

    const int abs_board_value = 500;

    EXPECT_EQ(evaluate_material(white_board), abs_board_value);
    EXPECT_EQ(evaluate_material(black_board), -abs_board_value);
}

TEST(EvaluateSquares, SimplePawn)
{
    Chessboard white_board = Chessboard("8/1P6/8/8/8/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/8/8/8/1p6/8");

    const int abs_board_value = 50;

    EXPECT_EQ(evaluate_squares(white_board), abs_board_value);
    EXPECT_EQ(evaluate_squares(black_board), -abs_board_value);
}

TEST(EvaluateSquares, SimpleQueen)
{
    Chessboard white_board = Chessboard("8/8/8/8/Q7/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/q7/8/8/8/8");

    const int abs_board_value = 0;

    EXPECT_EQ(evaluate_squares(white_board), abs_board_value);
    EXPECT_EQ(evaluate_squares(black_board), -abs_board_value);
}

TEST(EvaluateSquares, SimpleRook)
{
    Chessboard white_board = Chessboard("8/7R/8/8/8/8/8/8");
    Chessboard black_board = Chessboard("8/8/8/8/8/8/7r/8");

    const int abs_board_value = 5;

    EXPECT_EQ(evaluate_squares(white_board), abs_board_value);
    EXPECT_EQ(evaluate_squares(black_board), -abs_board_value);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
