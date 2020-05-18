#pragma once

#include <cstdint>

namespace option_parser
{
    // TODO some information are already in the move,
    // even more will be added in move after refacto
    struct BoardState
    {
        // First 8 bits, catling flag
        uint8_t white_king_castling : 1;
        uint8_t white_queen_castling : 1;
        uint8_t black_king_castling : 1;
        uint8_t black_queen_castling : 1;

        // Second 8 bits, if a piece was eaten, where it is
        uint8_t piece_type : 3; // 2^3 = 8 > 6,
                                // 'P' | 'N' | 'B' | 'R' | 'Q' | 'K'
        uint8_t ate : 1; // If a piece was eaten
        uint8_t eaten_x : 3; // x, y are only 2^3 = 8 position
        uint8_t eaten_y : 3;

        // Third 8 bit, the en passange
        uint8_t en_passant_x : 3; // x, y are only 2^3 = 8 position
        uint8_t en_passant_y : 3;
        uint8_t en_passant : 1; // If there was an en passant

        // The start and end position (that will be reversed)
        //are already in the position
    };

    void handle_input_option(int argc, const char* argv[]);
}