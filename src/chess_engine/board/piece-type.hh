#pragma once

#include <array>
#include <optional>

#include "utils/utype.hh"

namespace board
{
    /* PieceType is an enum representing every possible
     * piece type present on the board. The char associated
     * with each value is the ascii char representing the
     * piece on the board */
    enum class PieceType
    {
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN,
        KING, // Last position allows better optimization (order-move)
    };

    using opt_piecetype_t = std::optional<PieceType>;

    /* Can be useful to iterate over PieceTypes */
    constexpr std::array<PieceType, 6> piecetype_array{
        PieceType::QUEEN,  PieceType::ROOK, PieceType::BISHOP,
        PieceType::KNIGHT, PieceType::PAWN, PieceType::KING};

    constexpr size_t nb_pieces = piecetype_array.size();

    inline PieceType char_to_piece(const char c)
    {
        switch (c)
        {
        case 'P':
            return PieceType::PAWN;
        case 'N':
            return PieceType::KNIGHT;
        case 'B':
            return PieceType::BISHOP;
        case 'R':
            return PieceType::ROOK;
        case 'Q':
            return PieceType::QUEEN;
        case 'K':
            return PieceType::KING;
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }

    constexpr char empty_cell_char = ' ';

    inline char piece_to_char(const PieceType piece)
    {
        constexpr std::array<char, nb_pieces> piece_chars = {'Q', 'R', 'B', 'N', 'P', 'K'};

        return piece_chars[utils::utype(piece)];
    }

} // namespace board
