#include "gtest/gtest.h"

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/rule.hh"

using namespace board;
using namespace rule;
using namespace std;

TEST(PossibleMove, Queen)
{
    // This is the subject example
    Chessboard board = Chessboard("8/8/8/3Q4/8/8/8/8");
    auto moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(27, moves.size());

    board = Chessboard("7Q/8/8/8/8/8/8/8");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(21, moves.size());

    board = Chessboard("6P1/8/8/3Q4/8/8/8/8");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(26, moves.size());

    board = Chessboard("6P1/8/8/1P1Q4/4P3/8/8/8");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(20, moves.size());

    board = Chessboard("3p2P1/8/4p3/1P1Q4/8/8/8/4P3");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(23, moves.size());

    board = Chessboard("8/8/8/8/4R3/8/7p/6pQ");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(4, moves.size());

    board = Chessboard("3b3P/8/5Q2/P7/3p4/2Q2R2/1P6/8");
    moves = generate_moves(board, PieceType::QUEEN);
    EXPECT_EQ(15 + 18, moves.size());
}

TEST(PossibleMove, Pawn)
{
    Chessboard board = Chessboard("8/4P3/8/8/8/8/8/8");
    auto moves = generate_pawn_moves(board);
    EXPECT_EQ(4, moves.size());

    board = Chessboard("2ppp1p1/PPPPPP2/7P/8/8/8/8/8");
    moves = generate_pawn_moves(board);
    EXPECT_EQ(4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 1, moves.size());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}