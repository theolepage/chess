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

    # FIMXE
    print(fen_and_ref_value_couples)
    print(fen_and_value_couples)

    return dissonances(fen_and_ref_value_couples, fen_and_value_couples)

print(evaluate_eval_fun(eval))
