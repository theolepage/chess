#include "option-parser.hh"

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <dlfcn.h>

#include "listener/listener-manager.hh"
#include "parsing/pgn_parser/pgn-parser.hh"
#include "listener/listener.hh"

using namespace boost::program_options;

namespace option_parser
{
    static void handle_dl_error(void)
    {
        fprintf(stderr, "%s\n", dlerror());
    }

    static void* safe_dl_call(void* result)
    {
        if (result == nullptr)
            handle_dl_error();
        dlerror(); // No error, need to clear the error buffer before next call
        return result;
    }

    static void load_listerners(std::vector<std::string> listeners_path, listener::ListenerManager& manager)
    {
        const char* symbol_name = "listener_create";
        for (const std::string& listener_path : listeners_path)
        {
            void* handle = safe_dl_call(dlopen(listener_path.c_str(), RTLD_NOW | RTLD_GLOBAL)); // TODO correct flags ?
            if (handle == nullptr)
                continue;
            void* symbol = safe_dl_call(dlsym(handle, symbol_name));
            if (symbol == nullptr)
                continue;
            listener::Listener* listener = reinterpret_cast<listener::Listener*(*)()>(symbol)();
            manager.add_listener(handle, listener);
        }
    }

    static void on_perft(std::string path)
    {
        std::cout << path << std::endl;
    }

    void handle_input_option(int argc, const char* argv[])
    {
        listener::ListenerManager manager;
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
                std::cout << desc << '\n'; // print help message
            else
            {
                if (vm.count("listeners"))
                    load_listerners(listeners_path, manager);
                if (vm.count("pgn"))
                    manager.play_pgn_moves(pgn_parser::parse_pgn(pgn_path));
                else if (vm.count("perft"))
                    on_perft(perft_path);
                else
                    manager.play_ai();
            }
        }
        catch (const error &ex)
        {
            //TODO triggered on bad option, what expected ?
            std::cerr << ex.what() << '\n';
        }
        manager.close_listeners();
    }
}