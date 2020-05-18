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

def index_by_key(list, elt, key=identity):
    list_with_key_applied = [key(list[i]) for i in range(len(list))]

    return list_with_key_applied.index(elt)

def distance(list1, list2, key=identity):
    distance = 0

    # We put the indexes difference to the square because the bigger the
    # difference is, the more we want to exacerbate it
    for i in range(len(list1)):
        distance += (index_by_key(list2, first(list1[i]), key=key) - i) ** 2

    return distance

def cmp_eval_functions(eval_fun_1, eval_fun_2):
    fen_and_value_couples_1 = [(fen, eval_fun_1(fen)) for fen in fen_strings]
    fen_and_value_couples_2 = [(fen, eval_fun_2(fen)) for fen in fen_strings]

    fen_and_value_couples_1.sort(key=second)
    fen_and_value_couples_2.sort(key=second)

    print(fen_and_value_couples_1)
    print(fen_and_value_couples_2)

    return distance(fen_and_value_couples_1, fen_and_value_couples_2, key=first)

def evaluate_eval_fun(eval_fun):
    return cmp_eval_functions(ref_eval, eval_fun)

print(evaluate_eval_fun(eval))
