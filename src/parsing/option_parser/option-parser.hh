#pragma once

#include <cstdint>

namespace option_parser
{
    struct BoardState
    {
        // First 8 bits, catling + eaten piece
        uint8_t white_king_castling : 1;
        uint8_t white_queen_castling : 1;
        uint8_t black_king_castling : 1;
        uint8_t black_queen_castling : 1;

        uint8_t piece_type : 3; // 2^3 = 8 > 6, 'P' | 'N' | 'B' | 'R' | 'Q' | 'K'
        uint8_t ate : 1; // If a piece was eaten

        // Second 8 bit, the en passange
        uint8_t x : 3; // x, y are only 2^3 = 8 position
        uint8_t y : 3;
        uint8_t en_passant : 1; // If there was an en passant

        // The start and end position (that will be reversed) are already in the position
    };

    void handle_input_option(int argc, const char* argv[]);
}