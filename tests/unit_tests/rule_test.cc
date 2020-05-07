#include "gtest/gtest.h"

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/rule.hh"

using namespace board;
using namespace rule;

TEST(rule, get_pieces_positions_king)
{
    Chessboard board = Chessboard();

    std::vector<Position> res = get_pieces_positions(board, PieceType::KING, Color::BLACK);

    EXPECT_EQ(res.size(), 1);
    EXPECT_EQ(res.at(0), Position(File::E, Rank::EIGHT));
}

TEST(rule, get_pieces_positions_rook)
{
    Chessboard board = Chessboard();

    std::vector<Position> res = get_pieces_positions(board, PieceType::ROOK, Color::WHITE);

    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res.at(0), Position(File::A, Rank::ONE));
    EXPECT_EQ(res.at(1), Position(File::H, Rank::ONE));
}

TEST(rule, get_positions_between_simple)
{
    Position a = Position(File::A, Rank::TWO);
    Position b = Position(File::H, Rank::TWO);

    std::vector<Position> res = get_positions_between(a, b);

    EXPECT_EQ(res.size(), 8);
}

TEST(rule, get_positions_between_hard)
{
    Position a = Position(File::A, Rank::SEVEN);
    Position b = Position(File::D, Rank::FOUR);

    std::vector<Position> res = get_positions_between(a, b);

    EXPECT_EQ(res.size(), 4);
    EXPECT_EQ(res.at(0), a);
    EXPECT_EQ(res.at(1), Position(File::B, Rank::SIX));
    EXPECT_EQ(res.at(2), Position(File::C, Rank::FIVE));
    EXPECT_EQ(res.at(3), b);
}

TEST(rule, get_pieces_between_simple)
{
    Chessboard board = Chessboard();
    Position a = Position(File::A, Rank::SEVEN);
    Position b = Position(File::D, Rank::FOUR);

    int res = count_pieces_between(board, a, b);

    EXPECT_EQ(res, 1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}