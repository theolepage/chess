#!/usr/bin/env python

import random

# FIXME
fen_strings = ["foo", "bar", "baz"]

def ref_eval(fen):
    # FIXME
    return random.random()

def eval(fen):
    # FIXME
    return random.random()

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

def fens_have_same_order(fen_couple, fen_and_value_couples_1, fen_and_value_couples_2):
    first_fen, second_fen = fen_couple

    first_fen_index_1 = index_by_key(fen_and_value_couples_1, first_fen, key=first)
    second_fen_index_1 = index_by_key(fen_and_value_couples_1, second_fen, key=first)

    first_fen_index_2 = index_by_key(fen_and_value_couples_2, first_fen, key=first)
    second_fen_index_2 = index_by_key(fen_and_value_couples_2, second_fen, key=first)

    return (first_fen_index_1 <= second_fen_index_1 and first_fen_index_2 <= second_fen_index_2) \
        or (second_fen_index_1 <= first_fen_index_1 and second_fen_index_2 <= first_fen_index_2)

# Assumes that the couples are sorted by value
def dissonances(fen_and_ref_value_couples, fen_and_value_couples):
    dissonances = []

    for fen_couple in n_choose_2(fen_strings):
        if not fens_have_same_order(fen_couple, fen_and_ref_value_couples, fen_and_value_couples):
            first_fen, second_fen = fen_couple

            first_fen_ref_index = index_by_key(fen_and_ref_value_couples, first_fen, key=first)
            second_fen_ref_index = index_by_key(fen_and_ref_value_couples, second_fen, key=first)

            first_fen_index = index_by_key(fen_and_value_couples, first_fen, key=first)
            second_fen_index = index_by_key(fen_and_value_couples, second_fen, key=first)

            dissonances.append({'fen_string_1': first_fen,
                                'fen_string_1_ref_value': second(fen_and_ref_value_couples[first_fen_ref_index]),
                                'fen_string_1_value': second(fen_and_value_couples[first_fen_index]),
                                'fen_string_2': second_fen,
                                'fen_string_2_ref_value': second(fen_and_ref_value_couples[second_fen_ref_index]),
                                'fen_string_2_value': second(fen_and_value_couples[second_fen_index])})

    return dissonances

def evaluate_eval_fun(eval_fun):
    fen_and_ref_value_couples = [(fen, ref_eval(fen)) for fen in fen_strings]
    fen_and_value_couples = [(fen, eval_fun(fen)) for fen in fen_strings]

    fen_and_ref_value_couples.sort(key=second)
    fen_and_value_couples.sort(key=second)

    # FIMXE
    print(fen_and_ref_value_couples)
    print(fen_and_value_couples)

    dissonance_list = dissonances(fen_and_ref_value_couples, fen_and_value_couples)

    return (len(dissonance_list), dissonance_list)

print(evaluate_eval_fun(eval))
