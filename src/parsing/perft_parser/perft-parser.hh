#pragma once

#include <vector>

#include "perft-object.hh"

namespace perft_parser
{
    PerftObject parse_perft(std::string input);

    FenObject parse_fen(std::vector<std::string>& splited_input);
}