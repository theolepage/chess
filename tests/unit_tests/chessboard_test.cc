#include "gtest/gtest.h"

#include <sstream>

#include "chess_engine/board/chessboard.hh"
#include "chess_engine/ai/utype.hh"

using namespace board;

#define EXPECT_PIECETYPE(SidePiece, PieceType) do {\
    EXPECT_EQ(SidePiece.first, PieceType);\
} while (0)

#define EXPECT_COLOR(SidePiece, Color) do {\
    EXPECT_EQ(SidePiece.second, Color);\
} while (0)

#define EXPECT_PIECE(SidePiece, PieceType, Color) do {\
    EXPECT_PIECETYPE(SidePiece, PieceType);\
    EXPECT_COLOR(SidePiece, Color);\
} while (0)

#define EXPECT_NO_PIECE(SidePiece) do {\
    EXPECT_FALSE(SidePiece.has_value());\
} while (0)


TEST(InitialPieces, PawnRankTypes)
{
    Chessboard virgin_board;

    const auto pawn_ranks = {Rank::TWO, Rank::SEVEN};

    for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
    {
        auto file = static_cast<File>(file_i);

        for (auto rank : pawn_ranks)
        {
            auto side_piece = virgin_board[Position(file, rank)].value();
            EXPECT_PIECETYPE(side_piece, PieceType::PAWN);
        }
    }
}

// Make sure that the ranks 1 and 8 contains the right piecetypes
TEST(InitialPieces, EndRankTypes)
{
    Chessboard virgin_board;

    const auto end_ranks = {Rank::ONE, Rank::EIGHT};

    const size_t nb_piecetypes = 5;

    const std::array<PieceType, nb_piecetypes> piecetypes =
        { PieceType::ROOK,
          PieceType::KNIGHT,
          PieceType::BISHOP,
          PieceType::QUEEN,
          PieceType::KING,
    };

    std::vector<File> rook_files = {File::A, File::H};
    std::vector<File> knight_files = {File::B, File::G};
    std::vector<File> bishop_files = {File::C, File::F};
    std::vector<File> queen_files = {File::D};
    std::vector<File> king_files = {File::E};

    const std::array<std::vector<File>, nb_piecetypes> files =
        { rook_files,
          knight_files,
          bishop_files,
          queen_files,
          king_files
    };

    auto count = 0;

    for (size_t i = 0; i < nb_piecetypes; i++)
        for (auto file : files[i])
            for (auto rank : end_ranks)
            {
                auto side_piece = virgin_board[Position(file, rank)].value();
                EXPECT_PIECETYPE(side_piece, piecetypes[i]);
                count++;
            }

    EXPECT_EQ(count, 16);
}

TEST(InitialPieces, Colors)
{
    Chessboard virgin_board;

    const auto white_ranks = {Rank::ONE, Rank::TWO};
    const auto black_ranks = {Rank::SEVEN, Rank::EIGHT};

    for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
    {
        auto file = static_cast<File>(file_i);

        for (auto rank : white_ranks)
        {
            auto white_side_piece = virgin_board[Position(file, rank)].value();
            EXPECT_COLOR(white_side_piece, Color::WHITE);
        }

        for (auto rank : black_ranks)
        {
            auto black_side_piece = virgin_board[Position(file, rank)].value();
            EXPECT_COLOR(black_side_piece, Color::BLACK);
        }
    }
}

TEST(InitialPieces, EmptyMid)
{
    Chessboard virgin_board;

    for (size_t rank_i = 2; rank_i < Chessboard::width - 2; rank_i++)
    {
        auto rank = static_cast<Rank>(rank_i);

        for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
        {
            auto file = static_cast<File>(file_i);

            auto opt_side_piece = virgin_board[Position(file, rank)];
            EXPECT_FALSE(opt_side_piece.has_value());
        }
    }
}

size_t count_pieces(const Chessboard& board)
{
    auto count = 0;

    for (size_t rank_i = 0; rank_i < Chessboard::width; rank_i++)
    {
        auto rank = static_cast<Rank>(rank_i);

        for (size_t file_i = 0; file_i < Chessboard::width; file_i++)
        {
            auto file = static_cast<File>(file_i);

            if (board[Position(file, rank)].has_value())
                count++;
        }
    }

    return count;
}

TEST(InitialPieces, PieceCount)
{
    Chessboard board;

    EXPECT_EQ(count_pieces(board), 32);
}

TEST(Constructor, FenString)
{
    // FIXME Black and white seems to be inverted in fen string parser
    GTEST_SKIP();
    Chessboard board = Chessboard("4B3/8/2Q1p3/1k3N2/8/b1n3R1/8/K7");

    std::stringstream ss;
    ss << board;

    std::string expected_string =
        "8         B      \n"
        "7                \n"
        "6     Q   p      \n"
        "5   k       N    \n"
        "4                \n"
        "3 b   n       R  \n"
        "2                \n"
        "1 K              \n"
        "  A B C D E F G H";

    EXPECT_EQ(ss.str(), expected_string);
}

TEST(Constructor, PerftObjectCastling1)
{
    Chessboard castling_test_board = Chessboard(parse_perft("8/8/8/8/8/8/8/8 w kQ - 0 0 0"));

    EXPECT_TRUE(castling_test_board.get_king_castling(Color::BLACK));
    EXPECT_TRUE(castling_test_board.get_queen_castling(Color::WHITE));
    EXPECT_FALSE(castling_test_board.get_king_castling(Color::WHITE));
    EXPECT_FALSE(castling_test_board.get_queen_castling(Color::BLACK));
}

TEST(Constructor, PerftObjectCastling2)
{
    Chessboard castling_test_board = Chessboard(parse_perft("8/8/8/8/8/8/8/8 w kQKq - 0 0 0"));

    EXPECT_TRUE(castling_test_board.get_king_castling(Color::BLACK));
    EXPECT_TRUE(castling_test_board.get_queen_castling(Color::WHITE));
    EXPECT_TRUE(castling_test_board.get_king_castling(Color::WHITE));
    EXPECT_TRUE(castling_test_board.get_queen_castling(Color::BLACK));
}

TEST(Constructor, PerftObjectTurn)
{
    Chessboard white_turn_board = Chessboard(parse_perft("8/8/8/8/8/8/8/8 w - - 0 0 0"));
    Chessboard black_turn_board = Chessboard(parse_perft("8/8/8/8/8/8/8/8 b - - 0 0 0"));

    EXPECT_TRUE(white_turn_board.get_white_turn());
    EXPECT_FALSE(black_turn_board.get_white_turn());
}

Move dummy_move(const Position& start, const Position& end, const PieceType& piece)
{
    return Move(start, end, piece, false, false, false, false, false, std::nullopt);
}

Move dummy_double_pawn_push_move(const Position& start, const Position& end)
{
    return Move(start, end, PieceType::PAWN, false, true, false, false, false, std::nullopt);
}

Move dummy_en_passant_move(const Position& start, const Position& end)
{
    return Move(start, end, PieceType::PAWN, true, false, false, false, true, std::nullopt);
}

Move dummy_capture_move(const Position& start, const Position& end, PieceType piecetype)
{
    return Move(start, end, piecetype, true, false, false, false, false, std::nullopt);
}

TEST(Checkboard, Copy)
{
    Chessboard board;
    Chessboard board_copy = board;

    auto start = Position(File::A, Rank::TWO);
    auto end = Position(File::A, Rank::THREE);

    board_copy.do_move(dummy_move(start, end, PieceType::PAWN));

    EXPECT_PIECE(board_copy[end].value(), PieceType::PAWN, Color::WHITE);
    EXPECT_FALSE(board_copy.get_white_turn());

    EXPECT_FALSE(board[end].has_value());
    EXPECT_TRUE(board.get_white_turn());
}

TEST(DoMove, SimplePawnMove)
{
    Chessboard board;

    auto start_pos = Position(File::A, Rank::TWO);
    auto end_pos = Position(File::A, Rank::THREE);

    EXPECT_TRUE(board.get_white_turn());

    //  Move the leftmost white pawn one rank up
    board.do_move(dummy_move(start_pos, end_pos, PieceType::PAWN));

    EXPECT_FALSE(board.get_white_turn());
    EXPECT_FALSE(board[start_pos].has_value());

    auto moved_side_piece = board[end_pos].value();
    EXPECT_COLOR(moved_side_piece, Color::WHITE);
    EXPECT_PIECETYPE(moved_side_piece, PieceType::PAWN);

    EXPECT_EQ(count_pieces(board), 32);
}

TEST(DoMove, DoublePawnPush)
{
    Chessboard board;

    auto start_pos = Position(File::B, Rank::TWO);
    auto start_pos_front = Position(File::B, Rank::THREE);
    auto end_pos = Position(File::B, Rank::FOUR);

    EXPECT_TRUE(board.get_white_turn());

    board.do_move(dummy_double_pawn_push_move(start_pos, end_pos));

    EXPECT_FALSE(board.get_white_turn());

    EXPECT_FALSE(board[start_pos].has_value());
    EXPECT_FALSE(board[start_pos_front].has_value());

    auto moved_side_piece = board[end_pos].value();
    EXPECT_COLOR(moved_side_piece, Color::WHITE);
    EXPECT_PIECETYPE(moved_side_piece, PieceType::PAWN);

    EXPECT_EQ(count_pieces(board), 32);
}

TEST(DoMove, SimpleCapture)
{
    Chessboard board;

    auto start_pos = Position(File::A, Rank::TWO);
    auto end_pos = Position(File::A, Rank::THREE);

    EXPECT_TRUE(board.get_white_turn());

    //  Move the leftmost white pawn one rank up
    board.do_move(dummy_move(start_pos, end_pos, PieceType::PAWN));

    EXPECT_FALSE(board.get_white_turn());

    EXPECT_FALSE(board[start_pos].has_value());

    auto moved_side_pawn = board[end_pos].value();
    EXPECT_COLOR(moved_side_pawn, Color::WHITE);
    EXPECT_PIECETYPE(moved_side_pawn, PieceType::PAWN);

    auto left_black_rook_pos = Position(File::A, Rank::EIGHT);

    board.do_move(dummy_capture_move(left_black_rook_pos, end_pos, PieceType::ROOK));

    EXPECT_TRUE(board.get_white_turn());

    EXPECT_FALSE(board[left_black_rook_pos].has_value());

    EXPECT_PIECE(board[end_pos].value(), PieceType::ROOK, Color::BLACK);

    EXPECT_EQ(count_pieces(board), 31);
}

TEST(DoMove, ForgetEnPassant)
{
    Chessboard board;

    auto white_pawn_start = Position(File::B, Rank::TWO);
    auto white_pawn_end = Position(File::B, Rank::FOUR);
    auto en_passant_pos = Position(File::B, Rank::THREE);

    EXPECT_FALSE(board.get_en_passant().has_value());

    board.do_move(dummy_double_pawn_push_move(white_pawn_start, white_pawn_end));

    EXPECT_EQ(board.get_en_passant().value(), en_passant_pos);

    auto black_knight_start = Position(File::G, Rank::EIGHT);
    auto black_knight_end = Position(File::F, Rank::SIX);

    board.do_move(dummy_move(black_knight_start, black_knight_end, PieceType::ROOK));

    EXPECT_FALSE(board.get_en_passant().has_value());
}

TEST(DoMove, EnPassantCapture)
{
    Chessboard board;

    auto white_pawn_start = Position(File::C, Rank::TWO);
    auto white_pawn_end = Position(File::C, Rank::FIVE);

    board.do_move(dummy_move(white_pawn_start, white_pawn_end, PieceType::PAWN));

    auto black_pawn_start = Position(File::B, Rank::SEVEN);
    auto black_pawn_end = Position(File::B, Rank::FIVE);
    auto en_passant_pos = Position(File::B, Rank::SIX);

    board.do_move(dummy_double_pawn_push_move(black_pawn_start, black_pawn_end));

    EXPECT_EQ(board.get_en_passant().value(), en_passant_pos);

    board.do_move(dummy_en_passant_move(white_pawn_end, en_passant_pos));

    EXPECT_FALSE(board.get_en_passant().has_value());

    EXPECT_EQ(count_pieces(board), 31);
}


TEST(Draw, InitialState)
{
    // FIXME
    GTEST_SKIP();
    Chessboard board;

    EXPECT_FALSE(board.is_draw());
}

TEST(Draw, Stalemate)
{
    // FIXME
    GTEST_SKIP();
    // This is the subject example
    Chessboard board = Chessboard("8/8/8/1k6/8/b1n5/8/K7");

    EXPECT_TRUE(board.is_draw());
}

// NOTE Will not pass anymore if we implement the threefold repetition bonus
TEST(Draw, FiftyLastTurns1)
{
    // FIXME
    GTEST_SKIP();
    Chessboard board;

    auto white_rook_pos_1 = Position(File::B, Rank::ONE);
    auto white_rook_pos_2 = Position(File::A, Rank::THREE);
    auto white_1_to_2 = dummy_move(white_rook_pos_1, white_rook_pos_2, PieceType::ROOK);
    auto white_2_to_1 = dummy_move(white_rook_pos_2, white_rook_pos_1, PieceType::ROOK);

    auto black_rook_pos_1 = Position(File::G, Rank::EIGHT);
    auto black_rook_pos_2 = Position(File::H, Rank::SIX);
    auto black_1_to_2 = dummy_move(black_rook_pos_1, black_rook_pos_2, PieceType::ROOK);
    auto black_2_to_1 = dummy_move(black_rook_pos_2, black_rook_pos_1, PieceType::ROOK);

    for (auto turn = 0; turn < 50; turn++)
    {
        EXPECT_FALSE(board.is_draw());

        if (board.get_white_turn())
        {
            if (board[white_rook_pos_1].has_value())
                board.do_move(white_1_to_2);
            else
                board.do_move(white_2_to_1);
        }
        else
        {
            if (board[black_rook_pos_1].has_value())
                board.do_move(black_1_to_2);
            else
                board.do_move(black_2_to_1);
        }
    }

    EXPECT_TRUE(board.is_draw());
}

// NOTE Will not pass anymore if we implement the threefold repetition bonus
TEST(Draw, FiftyLastTurns2)
{
    // FIXME
    GTEST_SKIP();
    Chessboard board;

    auto white_rook_pos_1 = Position(File::B, Rank::ONE);
    auto white_rook_pos_2 = Position(File::A, Rank::THREE);
    auto white_1_to_2 = dummy_move(white_rook_pos_1, white_rook_pos_2, PieceType::ROOK);
    auto white_2_to_1 = dummy_move(white_rook_pos_2, white_rook_pos_1, PieceType::ROOK);

    auto black_rook_pos_1 = Position(File::G, Rank::EIGHT);
    auto black_rook_pos_2 = Position(File::H, Rank::SIX);
    auto black_1_to_2 = dummy_move(black_rook_pos_1, black_rook_pos_2, PieceType::ROOK);
    auto black_2_to_1 = dummy_move(black_rook_pos_2, black_rook_pos_1, PieceType::ROOK);

    for (auto turn = 0; turn < 49; turn++)
    {
        EXPECT_FALSE(board.is_draw());

        if (board.get_white_turn())
        {
            if (board[white_rook_pos_1].has_value())
                board.do_move(white_1_to_2);
            else
                board.do_move(white_2_to_1);
        }
        else
        {
            if (board[black_rook_pos_1].has_value())
                board.do_move(black_1_to_2);
            else
                board.do_move(black_2_to_1);
        }
    }

    EXPECT_FALSE(board.get_white_turn());

    // black pawn move
    board.do_move(dummy_move(Position(File::A, Rank::SEVEN),
                             Position(File::A, Rank::SIX),
                             PieceType::PAWN));

    EXPECT_FALSE(board.is_draw());
}


// To start tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
