#pragma once

#include <vector>
#include <optional>

#include "color.hh"
#include "position.hh"
#include "fen-rank.hh"
#include "position.hh"

namespace perft_parser
{
    class FenObject
    {
    public:
        using side_piece_t = std::pair<board::PieceType, board::Color>;
        using opt_piece_t = std::optional<side_piece_t>;

        FenObject(std::vector<FenRank>& ranks,
                    board::Color& slide_to_move,
                    std::vector<char>& castling,
                    std::optional<board::Position> en_passant) : ranks_(ranks),
                                                          slide_to_move_(slide_to_move),
                                                          castling_(castling),
                                                          en_passant_target_(en_passant)
                                                          {}

        FenObject() = default;
        FenObject& operator=(const FenObject&) = default;

        opt_piece_t operator[](const board::Position& ) const
        {
            // TODO really do it
            return std::nullopt;
        }

        board::Color side_to_move_to_get(void) const { return slide_to_move_; }
        std::vector<char> castling_get(void) const { return castling_; }
        std::optional<board::Position> en_passant_target_get(void) const { return en_passant_target_; }


    private:
        std::vector<FenRank> ranks_;
        board::Color slide_to_move_;
        std::vector<char> castling_;
        std::optional<board::Position> en_passant_target_;
    };
}