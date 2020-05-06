#include "gtest/gtest.h"

#include "chessboard.hh"
#include "utype.hh"

using namespace board;

#define EXPECT_PIECETYPE(SidePiece, PieceType) do {\
    EXPECT_EQ(SidePiece.first, PieceType);\
} while (0)

#define EXPECT_COLOR(SidePiece, Color) do {\
    EXPECT_EQ(SidePiece.second, Color);\
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

// To start tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
