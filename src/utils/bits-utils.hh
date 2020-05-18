#pragma once

#include <cstdint>

uint64_t pop_lsb(uint64_t board)
{
  int lsbIndex = __builtin_ffsll(board) - 1;
  board &= board - 1;
  return lsbIndex;
}