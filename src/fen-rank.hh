#pragma once

#include <utility>
#include <vector>
#include <cctype>
#include <cassert>
#include <cstdlib>
#include <optional>

#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace perft_parser
{
    class FenRank
    {
    public:
        using side_piece_t = std::pair<board::PieceType, board::Color>;
        using opt_piece_t = std::optional<side_piece_t>;

        FenRank(const std::string& rank)
        {
            assert(rank.size() != 0);
            for (char c : rank)
            {
                if (isupper(c))
                {
                    pieces_.emplace_back(opt_piece_t(side_piece_t(getPiece(c), board::Color::WHITE)));
                }
                else if (isdigit(c))
                {
                    const int val = c - '0'; // Move index of value
                    for (int j = 0; j < val; ++j) pieces_.emplace_back(std::nullopt);
                }
                else
                {
                    assert(islower(c));
                    pieces_.emplace_back(opt_piece_t(side_piece_t(getPiece(c), board::Color::BLACK)));
                }
            }
        }

        opt_piece_t operator[](const board::File& file) const
        {
            return pieces_.at(static_cast<int>(file)); // TODO enum class arith
        }

    private:
        board::PieceType getPiece(char c)
        {
            c = tolower(c);
            switch(c)
            {
                case 'r':
                    return board::PieceType::ROOK;
                case 'n':
                    return board::PieceType::KNIGHT;
                case 'q':
                    return board::PieceType::QUEEN;
                case 'p':
                    return board::PieceType::PAWN;
                case 'k':
                    return board::PieceType::KING;
                case 'b':
                    return board::PieceType::BISHOP;
                default:
                    assert(false);
                    return board::PieceType::BISHOP;
            }
            assert(false);
            return board::PieceType::BISHOP;
        }
        std::vector<opt_piece_t> pieces_;
    };
}