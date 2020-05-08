#pragma once

#include <cstdint>
#include <optional>

#include "parsing/perft_parser/perft-object.hh"
#include "parsing/perft_parser/perft-parser.hh"
#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/move.hh"
#include "chess_engine/board/color.hh"
#include "chess_engine/board/piece-type.hh"
#include "chess_engine/board/position.hh"

namespace option_parser
{
    using side_piece_t = std::pair<board::PieceType, board::Color>;
    using opt_piece_t = std::optional<side_piece_t>;
    using opt_pos_t = std::optional<board::Position>;

    /*struct BoardState
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

    uint64_t get_perft_value(board::Chessboard& board, int depth) // TODO generate legal moves shoudld be const
    {
        if (depth == 0)
            return 1;
        
        std::vector<board::Move> move_list = board.generate_legal_moves(); // TODO reduce size based on depth

        if (depth == 1)
            return move_list.size();

        u_int64_t nodes = 0; // TODO same

        for (const board::Move& m : move_list) // TODO convert to indice
        {
            State state;
            {
                opt_piece_t eaten = board[m.end_get()];
                opt_pos_t en_passant = board.get_en_passant();
                board.get_en_passant();
                s.white_king_castling = board.get_king_castling(board::Color::WHITE);
                s.white_queen_castling = board.get_queen_castling(board::Color::WHITE);
                s.black_king_castling = board.get_king_castling(board::Color::BLACK);
                s.black_queen_castling = board.get_queen_castling(board::Color::BLACK);
                s.piece_type = ((eaten.has_value()) ? static_cast<uint8_t>(eaten.value().first()) : 0);
                s.ate = ((eaten.has_value()) ? 1 : 0);
                s.x = ((en_passant.has_value()) ? static_cast<uint8_t>(en_passant.value().get_file()) : 0);
                s.y = ((en_passant.has_value()) ? static_cast<uint8_t>(en_passant.value().get_rank()) : 0);
                s.en_passant = ((en_passant.has_value()) ? 1 : 0);
            }
            board.do_move(m);
            nodes += get_perft_value(board, depth - 1);
            board.undo_move(m, state);
        }

        return nodes;
    }*/
}