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

TEST(rule, count_pieces_between_simple)
{
    Chessboard board = Chessboard();
    Position a = Position(File::A, Rank::SEVEN);
    Position b = Position(File::D, Rank::FOUR);

    int res = count_pieces_between(board, a, b);

    EXPECT_EQ(res, 0);
}

TEST(rule, count_pieces_between_simple_2)
{
    Chessboard board = Chessboard();
    Position a = Position(File::C, Rank::SEVEN);
    Position b = Position(File::C, Rank::SIX);

    int res = count_pieces_between(board, a, b);

    EXPECT_EQ(0, res);
}

TEST(rule, get_authorized_pos_simple)
{
    Chessboard board = Chessboard();
    
    std::vector<Position> rook = get_authorized_pos(PieceType::ROOK, Position(File::A, Rank::ONE));
    std::vector<Position> knight = get_authorized_pos(PieceType::KNIGHT, Position(File::B, Rank::ONE));
    std::vector<Position> king = get_authorized_pos(PieceType::KING, Position(File::E, Rank::ONE));
    std::vector<Position> queen = get_authorized_pos(PieceType::QUEEN, Position(File::D, Rank::ONE));

    EXPECT_EQ(rook.size(), 14);
    EXPECT_EQ(knight.size(), 3);
    EXPECT_EQ(king.size(), 5);
    EXPECT_EQ(queen.size(), 21);
}

TEST(rule, get_possible_move_pawn_simple)
{
    Chessboard board = Chessboard();
    
    std::optional<Move> forward_1 = get_possible_move(board,
        PieceType::PAWN,
        Color::BLACK,
        Position(File::C, Rank::SEVEN),
        Position(File::C, Rank::SIX));

    std::optional<Move> forward_2 = get_possible_move(board,
        PieceType::PAWN,
        Color::BLACK,
        Position(File::C, Rank::SEVEN),
        Position(File::C, Rank::FIVE));

    std::optional<Move> forward_3 = get_possible_move(board,
        PieceType::PAWN,
        Color::BLACK,
        Position(File::C, Rank::FIVE),
        Position(File::C, Rank::THREE));

    EXPECT_EQ(forward_1.has_value(), true);
    EXPECT_EQ(forward_2.has_value(), true);
    EXPECT_EQ(forward_3.has_value(), false);
}

// To-Do: register castling
// To-Do: register promotion
// To-Do: generate_moves

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}