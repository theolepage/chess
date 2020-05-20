#pragma once

#include "chess_engine/board/chessboard.hh"

namespace ai
{
    constexpr size_t width = board::Chessboard::width;
    constexpr size_t nb_pieces = board::nb_pieces;

    using piece_square_table_t = std::array<int, width * width>;
    using piece_square_tables_t = std::array<piece_square_table_t, nb_pieces>;

    // QUEEN, ROOK, BISHOP, KNIGHT, PAWN, KING
    constexpr std::array<unsigned, nb_pieces> piecetype_values
    {
        900, 500, 330, 320, 100, 20000
    };

    // NOTE The tables are well ordered for black pieces
    // They should be symetrically accessed
    // (as if the ranks where reversed) for white pieces
    //
    //        File H -- File A
    // Rank 1
    //   |
    // Rank 8
    // This one is asymetric along the file axis
    constexpr piece_square_table_t black_piece_square_table_queen = {
        -20,-10,-10, -5, -5,-10,-10, -20,
        -10,  0,  0,  0,  0,  0,  0, -10,
        -10,  0,  5,  5,  5,  5,  0, -10,
         -5,  0,  5,  5,  5,  5,  0,  -5,
         -5,  0,  5,  5,  5,  5,  0,   0,
        -10,  0,  5,  5,  5,  5,  5, -10,
        -10,  0,  0,  0,  0,  5,  0, -10,
        -20,-10,-10, -5, -5,-10,-10, -20
    };

    constexpr piece_square_table_t black_piece_square_table_rook = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
    };

    constexpr piece_square_table_t black_piece_square_table_bishop = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };

    constexpr piece_square_table_t black_piece_square_table_knight = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    constexpr piece_square_table_t black_piece_square_table_pawn = {
        0,   0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,   5, 10, 25, 25, 10,  5,  5,
        0,   0,  0, 20, 20,  0,  0,  0,
        5,  -5,-10,  0,  0,-10, -5,  5,
        5,  10, 10,-20,-20, 10, 10,  5,
        0,   0,  0,  0,  0,  0,  0,  0
    };

    // FIXME There is also a king end game piece_square_table
    constexpr piece_square_table_t black_piece_square_table_king = {
        // king middle game
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };

    // NOTE Should follow the exact same order than piecetype_array
    // in piece-type.hh, ie:
    // QUEEN, ROOK, BISHOP, KNIGHT, PAWN, KING
    constexpr piece_square_tables_t black_piece_square_tables = {
        black_piece_square_table_queen,
        black_piece_square_table_rook,
        black_piece_square_table_bishop,
        black_piece_square_table_knight,
        black_piece_square_table_pawn,
        black_piece_square_table_king
    };

    // Return a symetric table of the one providen, along the rank axis
    constexpr piece_square_table_t
    generate_symetric_table(piece_square_table_t table)
    {
        piece_square_table_t symetric_table = table;

        for (size_t i = 0; i < table.size(); i++)
        {
            auto i_sym = width * (width - (i / width + 1)) + i % width;
            symetric_table[i] = table[i_sym];
        }

        return symetric_table;
    }

    constexpr piece_square_tables_t
    generate_symetric_tables (piece_square_tables_t tables)
    {
        piece_square_tables_t symetric_tables = tables;

        for (size_t i = 0; i < symetric_tables.size(); i++)
            symetric_tables[i] = generate_symetric_table(tables[i]);

        return symetric_tables;
    }

    constexpr piece_square_tables_t white_piece_square_tables =
        generate_symetric_tables(black_piece_square_tables);

    int evaluate_material(const board::Chessboard& board);
    int evaluate_squares(const board::Chessboard& board);
    int evaluate(const board::Chessboard& board);
}
