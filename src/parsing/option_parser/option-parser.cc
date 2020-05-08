#include "option-parser.hh"

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <dlfcn.h>

#include "listener/listener-manager.hh"
#include "parsing/pgn_parser/pgn-parser.hh"
#include "parsing/perft_parser/perft-object.hh"
#include "parsing/perft_parser/perft-parser.hh"
#include "listener/listener.hh"

using namespace boost::program_options;

namespace option_parser
{
    using side_piece_t = std::pair<board::PieceType, board::Color>;
    using opt_piece_t = std::optional<side_piece_t>;
    using opt_pos_t = std::optional<board::Position>;

    static void on_pgn(std::string path)
    {
        auto moves = pgn_parser::parse_pgn(path);
    }

    // Should kill program here
    static void handle_dl_error(void) // TODO do whaaat ?
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(0);
    }

    static void* safe_dl_call(void* result)
    {
        if (result == nullptr)
            handle_dl_error(); // Will kill the programm
        dlerror(); // No error, need to clear the error buffer before next call
        return result;
    }

    static void on_listerners(std::vector<std::string> listeners_path, listener::ListenerManager& manager)
    {
        const char* symbol_name = "listener_create";
        for (const std::string& listener_path : listeners_path)
        {
            void* handle = safe_dl_call(dlopen(listener_path.c_str(), RTLD_NOW | RTLD_GLOBAL)); // TODO correct flags ?
            void* symbol = safe_dl_call(dlsym(handle, symbol_name));
            listener::Listener* listener = reinterpret_cast<listener::Listener*(*)()>(symbol)();
            manager.add_listener(handle, listener);
        }
    }

    static std::string get_file_content(const std::string& path)
    {
        std::ifstream t(path);
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    }

    // Would be better in it's own file but fails when I try
    static uint64_t get_perft_value(board::Chessboard& board, int depth) // TODO generate legal moves shoudld be const
    {
        if (depth == 0)
            return 1;
        
        std::vector<board::Move> move_list = board.generate_legal_moves(); // TODO reduce size based on depth

        if (depth == 1)
            return move_list.size();

        u_int64_t nodes = 0; // TODO same

        for (const board::Move& m : move_list) // TODO convert to indice
        {
            BoardState state; // Scope used to destroy eaten & en_passant object
            {
                opt_piece_t eaten = board[m.end_get()];
                opt_pos_t en_passant = board.get_en_passant();
                state.white_king_castling = board.get_king_castling(board::Color::WHITE);
                state.white_queen_castling = board.get_queen_castling(board::Color::WHITE);
                state.black_king_castling = board.get_king_castling(board::Color::BLACK);
                state.black_queen_castling = board.get_queen_castling(board::Color::BLACK);
                state.piece_type = ((eaten.has_value()) ? static_cast<uint8_t>(eaten.value().first) : 0);
                state.ate = ((eaten.has_value()) ? 1 : 0);
                state.x = ((en_passant.has_value()) ? static_cast<uint8_t>(en_passant.value().get_file()) : 0);
                state.y = ((en_passant.has_value()) ? static_cast<uint8_t>(en_passant.value().get_rank()) : 0);
                state.en_passant = ((en_passant.has_value()) ? 1 : 0);
            }
            board.do_move(m);
            nodes += get_perft_value(board, depth - 1);
            board.undo_move(m, state);
        }

        return nodes;
    }

    static void on_perft(std::string path)
    {
        perft_parser::PerftObject perftobj = perft_parser::parse_perft(get_file_content(path));
        board::Chessboard chessboard(perftobj);
        std::cout << get_perft_value(chessboard, perftobj.get_depth()) << std::endl;
    }

    static void start_ai(void)
    {
        std::cout << "The magic happens here" << std::endl;
    }

    void handle_input_option(int argc, const char* argv[])
    {
        listener::ListenerManager manager; // TODO where to put this and close it
        try
        {
            std::string pgn_path, perft_path;
            std::vector<std::string> listeners_path;
            
            options_description desc{"Allowed options"};
            desc.add_options()
            ("help,h", "show usage")
            ("pgn", value<std::string>(&pgn_path), "path to the PGN game file")
            ("listeners,l", value<std::vector<std::string>>(&listeners_path), "list of paths to listener plugins")
            ("perft", value<std::string>(&perft_path), "path to a perft file");

            variables_map vm;
            store(parse_command_line(argc, argv, desc), vm);
            notify(vm);

            if (vm.count("help"))
                std::cout << desc << '\n';
            if (vm.count("pgn"))
                on_pgn(pgn_path);
            if (vm.count("listeners"))
                on_listerners(listeners_path, manager);
            if (vm.count("perft"))
                on_perft(perft_path);
        }
        catch (const error &ex)
        {
            //TODO triggered on bad option, what expected ?
            std::cerr << ex.what() << '\n';
        }
        if (argc == 1) // No option
        {
            start_ai();
        }
        manager.close_listeners();
    }
}