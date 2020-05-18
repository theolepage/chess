#!/usr/bin/env python

import os
import chess
from fen_eval import fen_eval

def which(bin_name):
    path = os.getenv('PATH')
    for bin_path in path.split(os.path.pathsep):
        bin_path = os.path.join(bin_path, bin_name)
        if os.path.exists(bin_path) and os.access(bin_path, os.X_OK):
            return bin_path

stockfish_path = which('stockfish')
chessengine_path = '../chessengine'

# FIXME
fen_strings = ["k7/pp3ppp/8/3p4/8/2P4P/PP3PP1/K3R3", # deadly open file for black king
"k7/pp3ppp/2p1p3/3p4/8/2P4P/PP3PP1/K3R3" # precedent fen with closed black pawn shied
]

# This is usefull to normalize Score objects to integers
# and should be a large value
mate_score = 1000000000

# There is a total order defined on centi-pawn and mate scores.
# >>> from chess.engine import Cp, Mate, MateGiven
# >>> Mate(-0) < Mate(-1) < Cp(-50) < Cp(200) < Mate(4) < Mate(1) < MateGiven
# True
def score_to_centipawns(pov_score):
    score = pov_score.relative

    if type(score) == chess.engine.Cp:
        assert score.score() < mate_score

    return score.score(mate_score=mate_score)

def ref_eval(fen):
    return score_to_centipawns(fen_eval(fen, stockfish_path))

def eval(fen):
    return score_to_centipawns(fen_eval(fen, chessengine_path))

def identity(x):
    return x

def first(couple):
    return couple[0]

def second(couple):
    return couple[1]

# Construct a list of all the possible unordered couples in list
def n_choose_2(list):
    if len(list) == 0:
        return []

    couples_with_first = []

    first, rest = list[0], list[1:]

    for elt in rest:
        couples_with_first.append((first, elt))

    return couples_with_first + n_choose_2(rest)

def index_by_key(list, elt, key=identity):
    list_with_key_applied = [key(list[i]) for i in range(len(list))]

    return list_with_key_applied.index(elt)

def get_choice_dissonance(fen_couple, fen_and_ref_value_couples, fen_and_value_couples):
    first_fen, second_fen = fen_couple

    first_fen_ref_index = index_by_key(fen_and_ref_value_couples, first_fen, key=first)
    second_fen_ref_index = index_by_key(fen_and_ref_value_couples, second_fen, key=first)

    first_fen_index = index_by_key(fen_and_value_couples, first_fen, key=first)
    second_fen_index = index_by_key(fen_and_value_couples, second_fen, key=first)

    first_fen_ref_value = second(fen_and_ref_value_couples[first_fen_ref_index])
    second_fen_ref_value = second(fen_and_ref_value_couples[second_fen_ref_index])

    first_fen_value = second(fen_and_value_couples[first_fen_index])
    second_fen_value = second(fen_and_value_couples[second_fen_index])

    if (first_fen_ref_index <= second_fen_ref_index and first_fen_index <= second_fen_index) \
       or (second_fen_ref_index <= first_fen_ref_index and second_fen_index <= first_fen_index):
        return None

    return {'fen_string_1': first_fen,
            'fen_string_1_ref_value': first_fen_ref_value,
            'fen_string_1_value': first_fen_value,
            'fen_string_2': second_fen,
            'fen_string_2_ref_value': second_fen_ref_value,
            'fen_string_2_value': second_fen_value}

def get_winner_dissonance(fen, fen_and_ref_value_couples, fen_and_value_couples):
    fen_ref_index = index_by_key(fen_and_ref_value_couples, fen, key=first)
    fen_index = index_by_key(fen_and_value_couples, fen, key=first)

    fen_ref_value = second(fen_and_ref_value_couples[fen_ref_index])
    fen_value = second(fen_and_value_couples[fen_index])

    if (fen_ref_value <= 0 and fen_value <= 0) or (fen_ref_value > 0 and fen_value > 0):
        return None

    return {'fen_string': fen,
            'fen_string_ref_value': fen_ref_value,
            'fen_string_value': fen_value}


# Assumes that the couples are sorted by value
def dissonances(fen_and_ref_value_couples, fen_and_value_couples):
    winner_dissonances = []
    choice_dissonances = []

    for fen in fen_strings:
        winner_dissonance = get_winner_dissonance(fen, fen_and_ref_value_couples, fen_and_value_couples)
        if winner_dissonance:
            winner_dissonances.append(winner_dissonance)

    for fen_couple in n_choose_2(fen_strings):
        choice_dissonance = get_choice_dissonance(fen_couple, fen_and_ref_value_couples, fen_and_value_couples)
        if choice_dissonance:
            choice_dissonances.append(choice_dissonance)

    return {'winner_dissonances': winner_dissonances, 'choice_dissonances': choice_dissonances}

def evaluate_eval_fun(eval_fun):
    fen_and_ref_value_couples = [(fen, ref_eval(fen)) for fen in fen_strings]
    fen_and_value_couples = [(fen, eval_fun(fen)) for fen in fen_strings]

    fen_and_ref_value_couples.sort(key=second)
    fen_and_value_couples.sort(key=second)

    print(fen_and_ref_value_couples)
    print(fen_and_value_couples)

    return dissonances(fen_and_ref_value_couples, fen_and_value_couples)

print(evaluate_eval_fun(eval))
