#ifndef INCLUDE_BRANCHPREDICTOR_H_
#define INCLUDE_BRANCHPREDICTOR_H_

#include "logarithm.h"
#include <ac_int.h>

template <int BITS, int ENTRIES> class BitBranchPredictor {
  static const int LOG_ENTRIES = log2const<ENTRIES>::value;
  static const int T_START     = (1 << BITS) - 1;
  static const int T_FINAL     = (1 << BITS) >> 1;
  static const int NT_START    = 0;
  static const int NT_FINAL    = T_FINAL - 1;

public:
  ac_int<BITS, false> table[ENTRIES];
  BitBranchPredictor()
  {
    for (int i = 0; i < ENTRIES; i++) {
      table[i] = T_START;
    }
  }

  void update(ac_int<32, false> pc, bool isBranch)
  {
    ac_int<LOG_ENTRIES, false> index = pc.slc<LOG_ENTRIES>(0);
    if (isBranch) {
      table[index] += table[index] != T_START ? 1 : 0;
    } else {
      table[index] -= table[index] != NT_START ? 1 : 0;
    }
  }

  void process(ac_int<32, false> pc, bool& isBranch)
  {
    ac_int<LOG_ENTRIES, false> index = pc.slc<LOG_ENTRIES>(0);
    isBranch                         = table[index] >= T_FINAL;
  }
};

using BranchPredictor = BitBranchPredictor<2, 4>;

#endif /* INCLUDE_BRANCHPREDICTOR_H_ */
