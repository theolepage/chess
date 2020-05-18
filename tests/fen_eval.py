from argparse import ArgumentParser
from argparse import RawDescriptionHelpFormatter
from pathlib import Path
import os
import subprocess as sp
import chess
import chess.engine
import sys

def dummy_fen_to_perft(fen_string):
    return fen_string + " b - - 0 1"

def perft_eval(perft_string, engine_path):
    """ give the evaluation of the perft board from the given engine (in centipawns)"""

    # Load board from perft_string
    board = chess.Board(fen=perft_string)

    # Load engine
    engine = chess.engine.SimpleEngine.popen_uci(engine_path)

    # Analyse position of the board from the engine:
    info = engine.analyse(board, chess.engine.Limit(time=1))

    # stop the engine
    engine.quit()

    # return evaluation score in centipawns
    return info["score"]

def fen_eval(fen_string, engine_path):
    return perft_eval(dummy_fen_to_perft, engine_path)

if __name__ == "__main__":
    """ This script launch fen_eval """

    # Set all arguments that can be used by the script
    parser = ArgumentParser(description= "fen_eval : give the evaluation of the fen from specific engine (in centipawns)",
            formatter_class = RawDescriptionHelpFormatter)
    parser.add_argument("engine", metavar="ENGINE")
    parser.add_argument("fen", metavar="FEN")

    args = parser.parse_args()

    sys.exit(fen_eval(args.fen, args.engine));
