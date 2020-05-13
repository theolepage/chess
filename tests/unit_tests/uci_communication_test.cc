#include "gtest/gtest.h"

#include <string>

#include "chess_engine/board/move.hh"
#include "chess_engine/board/position.hh"
#include "chess_engine/board/piece-type.hh"
#include "parsing/pgn_parser/ebnf-parser.hh"

using namespace board;
using namespace std;
using namespace pgn_parser;

TEST(EBNFParser,MoveToString)
{
    Move f(Position(File::A, Rank::TWO), Position(File::E, Rank::THREE), PieceType::ROOK, false, false, false, false, false);
    string f_expected = "a2e3";
    EXPECT_EQ(f_expected, move_to_string(f));

    Move s(Position(File::C, Rank::EIGHT), Position(File::F, Rank::FOUR), PieceType::ROOK, false, false, false, false, false, PieceType::ROOK);
    string s_expected = "c8f4r";
    EXPECT_EQ(s_expected, move_to_string(s));
}

TEST(EBNFParser, StringToMovePromotion)
{
   Chessboard board("5p2/4P3/8/8/8/8/8/8");

   string move = "e7f8n";

   Move m = string_to_move(board, move);
   EXPECT_TRUE(m.capture_get());
   EXPECT_TRUE(m.promotion_get().has_value());
   EXPECT_EQ(PieceType::KNIGHT, m.promotion_get().value());
   EXPECT_EQ(Position(File::E, Rank::SEVEN), m.start_get());
   EXPECT_EQ(Position(File::F, Rank::EIGHT), m.end_get());
   EXPECT_FALSE(m.double_pawn_push_get());
   EXPECT_FALSE(m.queen_castling_get());
   EXPECT_FALSE(m.king_castling_get());
   EXPECT_FALSE(m.en_passant_get());
}

TEST(EBNFParser, StringToMoveCastling)
{
   Chessboard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R");

   string move = "e1g1";

   Move m = string_to_move(board, move);
   EXPECT_FALSE(m.capture_get());
   EXPECT_FALSE(m.promotion_get().has_value());
   EXPECT_EQ(Position(File::E, Rank::ONE), m.start_get());
   EXPECT_EQ(Position(File::G, Rank::ONE), m.end_get());
   EXPECT_FALSE(m.double_pawn_push_get());
   EXPECT_FALSE(m.queen_castling_get());
   EXPECT_TRUE(m.king_castling_get());
   EXPECT_FALSE(m.en_passant_get());
}

TEST(EBNFParser, StringToMoveEnPassant)
{
   Chessboard board("rnbqkbnr/2pppppp/8/p7/Pp6/8/1PPPPPPP/RNBQK2R");

   string move = "b4a3";

   Move m = string_to_move(board, move);
   EXPECT_TRUE(m.capture_get());
   EXPECT_FALSE(m.promotion_get().has_value());
   EXPECT_EQ(Position(File::B, Rank::FOUR), m.start_get());
   EXPECT_EQ(Position(File::A, Rank::THREE), m.end_get());
   EXPECT_FALSE(m.double_pawn_push_get());
   EXPECT_FALSE(m.queen_castling_get());
   EXPECT_FALSE(m.king_castling_get());
   EXPECT_TRUE(m.en_passant_get());
}

TEST(UCIMove, BasePositionNoMove)
{
    Chessboard board;
    Chessboard expected;

    add_move_to_board(board, "position startpos");
    EXPECT_EQ(expected, board);
}

TEST(UCIMove, basePositionMove)
{
    Chessboard board;
    Chessboard expected;
    expected.do_move(Move(Position(File::A, Rank::TWO), Position(File::A, Rank::FOUR), PieceType::PAWN, false, true, false, false, false));

    add_move_to_board(board, "position startpos moves a2a4");
    EXPECT_EQ(expected, board);
}

TEST(UCIMove, fenStrNoMove)
{
    string str = "r1bqkb1r/pp2pppp/2np1n2/6B1/3NP3/2N5/PPP2PPP/R2QKB1R b KQkq - 5 6";
    std::vector<std::string> tokens;
    boost::split(tokens, str, boost::is_any_of(" "));
    Chessboard expected(parse_fen(tokens));
    Chessboard board;

    add_move_to_board(board, "position fen r1bqkb1r/pp2pppp/2np1n2/6B1/3NP3/2N5/PPP2PPP/R2QKB1R b KQkq - 5 6");

    EXPECT_EQ(expected, board);
}

TEST(UCMove, fenStrMove)
{
    string str = "r1bqkb1r/pp2pppp/2np1n2/6B1/3NP3/2N5/PPP2PPP/R2QKB1R b KQkq - 5 6";
    std::vector<std::string> tokens;
    boost::split(tokens, str, boost::is_any_of(" "));
    Chessboard expected(parse_fen(tokens));
    expected.do_move(Move(Position(File::A, Rank::TWO), Position(File::A, Rank::THREE), PieceType::PAWN, false, false, false, false, false));

    Chessboard board;
    add_move_to_board(board, "position fen r1bqkb1r/pp2pppp/2np1n2/6B1/3NP3/2N5/PPP2PPP/R2QKB1R b KQkq - 5 6 moves a2a3");

    EXPECT_EQ(expected, board);
}

// To start tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}