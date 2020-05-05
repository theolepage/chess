#include "option-parser.hh"

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <dlfcn.h>

#include "listener-manager.hh"
#include "pgn-parser.hh"
#include "listener.hh"

using namespace boost::program_options;

namespace option_parser
{
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

    static void on_perft(std::string path)
    {
        std::cout << path << std::endl;
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
            else if (vm.count("pgn"))
                on_pgn(pgn_path);
            else if (vm.count("listeners"))
                on_listerners(listeners_path, manager);
            else if (vm.count("perft"))
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