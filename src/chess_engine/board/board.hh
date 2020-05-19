#pragma once

#include <cstdint>

#include "entity/position.hh"
#include "entity/piece-type.hh"
#include "bitboard.hh"

class Board
{
public:
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>;
    using opt_pos_t = std::optional<Position>;

    opt_piece_t operator[](const Position& pos) const;

    void set_piece(const Position& pos,
                   const PieceType piecetype,
                   const Color color);

    void unset_piece(const Position& pos,
                     const PieceType piecetype,
                     const Color color);

    void move_piece(const Position& start,
                    const Position& end,
                    const PieceType piecetype,
                    const Color color);

private:
    Bitboard whites_;
    Bitboard blacks_;
    Bitboard rooks_;
    Bitboard pawns_;
    Bitboard knights_;
    Bitboard bishops_;
    Bitboard queens_;
    Bitboard kings_;

};