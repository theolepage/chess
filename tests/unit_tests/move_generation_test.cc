#include "gtest/gtest.h"

#include "chess_engine/board/entity/color.hh"
#include "chess_engine/board/entity/piece-type.hh"
#include "chess_engine/board/entity/position.hh"
#include "chess_engine/board/move-generation.hh"

using namespace board;

TEST(move_generation_test, simple_bishop)
{
    Chessboard b("8/8/8/3B4/8/8/8/8");
    auto res = move_generation::generate_bishop_moves(b);
    EXPECT_EQ(13, res.size());

    Chessboard b1("B7/8/8/8/8/8/8/8");
    res = move_generation::generate_bishop_moves(b1);
    EXPECT_EQ(7, res.size());

    Chessboard b2("8/8/8/8/8/8/4B3/8");
    res = move_generation::generate_bishop_moves(b2);
    EXPECT_EQ(9, res.size());
}

TEST(move_generation_test, simple_bishop_with_blockers)
{
    Chessboard b("8/8/8/8/2p3P1/8/4B3/8");
    auto res = move_generation::generate_bishop_moves(b);
    EXPECT_EQ(5, res.size());

    Chessboard b1("8/8/p7/4p2P/8/3p4/3pB2p/3P1p2");
    res = move_generation::generate_bishop_moves(b1);
    EXPECT_EQ(4, res.size());
}

TEST(move_generation_test, simple_rook)
{
    Chessboard b("8/8/8/3R4/8/8/8/8");
    auto res = move_generation::generate_rook_moves(b);
    EXPECT_EQ(14, res.size());

    Chessboard b1("8/8/8/8/8/8/8/R7");
    res = move_generation::generate_rook_moves(b1);
    EXPECT_EQ(14, res.size());

    Chessboard b2("8/8/8/8/8/1R6/8/8");
    res = move_generation::generate_rook_moves(b2);
    EXPECT_EQ(14, res.size());
}

TEST(move_generation_test, simple_rook_with_blockers)
{
    Chessboard b("8/8/1p6/8/8/8/pR4P1/1P6");
    auto res = move_generation::generate_rook_moves(b);
    EXPECT_EQ(9, res.size());

    Chessboard b1("4pkP1/1n2R1q1/3pP3/6Q1/8/8/6P1/8");
    res = move_generation::generate_rook_moves(b1);
    EXPECT_EQ(6, res.size());

    Chessboard b2("8/8/1n1n4/1qqqqR2/1n1r1Q2/1n1PBN2/1nB2B2/8", Color::BLACK);
    res = move_generation::generate_rook_moves(b2);
    EXPECT_EQ(4, res.size());

    Chessboard b3("R1p3p1/8/8/8/8/8/8/r3R3", Color::BLACK);
    res = move_generation::generate_rook_moves(b3);
    EXPECT_EQ(11, res.size());

    Chessboard b4("r1p3p1/8/8/8/8/8/8/r3r3", Color::BLACK);
    res = move_generation::generate_rook_moves(b4);
    EXPECT_EQ(9, res.size());

    Chessboard b5("1rp3p1/8/8/8/8/8/1r2r3/8", Color::BLACK);
    res = move_generation::generate_rook_moves(b5);
    EXPECT_EQ(9, res.size());

    Chessboard b6("2p3p1/1r6/8/8/8/8/1r2r3/8", Color::BLACK);
    res = move_generation::generate_rook_moves(b6);
    EXPECT_EQ(8, res.size());

    Chessboard b7("8/1r6/8/8/8/8/1r2r3/8", Color::BLACK);
    res = move_generation::generate_rook_moves(b7);
    EXPECT_EQ(8, res.size());
}

TEST(move_generation_test, simple_queen)
{
    Chessboard b("8/8/8/3Q4/8/8/8/8");
    auto res = move_generation::generate_queen_moves(b);
    EXPECT_EQ(27, res.size());

    Chessboard b1("8/8/8/8/8/8/8/q7", Color::BLACK);
    res = move_generation::generate_queen_moves(b1);
    EXPECT_EQ(21, res.size());

    Chessboard b2("R1p3p1/8/8/8/8/8/8/q3R3", Color::BLACK);
    res = move_generation::generate_queen_moves(b2);
    EXPECT_EQ(18, res.size());

    // Chessboard b2("R1p1q1p1/8/8/8/8/8/8/q3R3", Color::BLACK);
    // res = move_generation::generate_queen_moves(b2);
    // EXPECT_EQ(34, res.size());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}