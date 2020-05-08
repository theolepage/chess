#pragma once

#include <optional>

#include "chess_engine/board/color.hh"
#include "chess_engine/board/piece-type.hh"
#include "chess_engine/board/position.hh"
#include "chess_engine/board/move.hh"
#include "report-type.hh"

namespace board
{
    /* The move symbolize a movement of a piece. Every turn should
     * correspond to one move. It contains every informations related
     * to the action */
    class PgnMove final
    {
    public:
        /* initialize it with PgnMove::opt_piece_t { [PieceType] } */
        using opt_piece_t = std::optional<PieceType>;

        PgnMove(const Position &start, const Position &end, PieceType piece,
                bool capture, ReportType report,
                const opt_piece_t &promotion = std::nullopt);

        PgnMove(const Position &start, const Position &end,
                PieceType piece, bool capture, ReportType report,
                const PgnMove::opt_piece_t &promotion,
                bool queen_castling, bool king_castling);

        /*! \brief Generate a castling given a color and a side */
        static PgnMove generate_castling(bool queen_side, Color color);

        void report_set(ReportType report);

        board::Move to_Move() const;
        board::Move to_Move(const board::Move previous_move) const;

    private:
        // The original position of the piece
        const Position start_;

        // The destination of the piece
        const Position end_;

        // The type of the moving piece
        PieceType piece_;

        // if promotion, contains the piecetype generated by the promotion
        opt_piece_t promotion_;

        // if the move is a capture
        bool capture_;

        // if the move is a queen_castling
        bool queen_castling_;

        // if the move is a king_castling
        bool king_castling_;

        // if the move is a double_pawn_push
        bool double_pawn_push_;

        // type of report given by the pgn file
        ReportType report_;
    };
} // namespace board
