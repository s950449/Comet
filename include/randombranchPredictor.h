#ifndef INCLUDE_BRANCHPREDICTOR_H_
#define INCLUDE_BRANCHPREDICTOR_H_

#include "logarithm.h"
#include <ac_int.h>

#ifdef DEBUG
#include <assert.h>
#include <iostream>
#include <queue>
#endif

template <class T> class BranchPredictorWrapper {
#ifdef DEBUG
  int missPredictions = 0;
  int processCount    = 0;
  int updateCount     = 0;
  bool predictions[2];
  int front = 0, back = 0;
#endif

public:
  void update(ac_int<32, false> pc, bool isBranch)
  {
    static_cast<T*>(this)->_update(pc, isBranch);
#ifdef DEBUG
    updateCount++;
    assert(updateCount + 1 >= processCount);
    bool pred = predictions[front];
    front ^= 1;
    missPredictions += isBranch == pred ? 0 : 1;
    std::cout << "pc: " << pc << "\n"
              << "branch: " << isBranch << "\n"
              << "predict: " << pred << "\n"
              << "miss rate: " << (float)missPredictions / updateCount << "\n";

#endif
  }

  void process(ac_int<32, false> pc, bool& isBranch)
  {
    static_cast<T*>(this)->_process(pc, isBranch);
#ifdef DEBUG
    processCount++;
    predictions[back] = isBranch;
    back ^= 1;
#endif
  }

  void undo()
  {
#ifdef DEBUG
    if (processCount > updateCount) {
      processCount--;
      front ^= 1;
    }
#endif
  }
};

template <int BITS, int ENTRIES>
class BitBranchPredictor : public BranchPredictorWrapper<BitBranchPredictor<BITS, ENTRIES> > {
  static const int LOG_ENTRIES = log2const<ENTRIES>::value;
  static const int NT_START    = (1 << BITS) - 1;
  static const int NT_FINAL    = (1 << BITS) >> 1;
  static const int T_START     = 0;
  static const int T_FINAL     = NT_FINAL - 1;

  ac_int<BITS, false> table[ENTRIES];

public:
  BitBranchPredictor()
  {
    for (int i = 0; i < ENTRIES; i++) {
      table[i] = T_START;
    }
  }

  void _update(ac_int<32, false> pc, bool isBranch)
  {
    ac_int<LOG_ENTRIES, false> index = pc.slc<LOG_ENTRIES>(2);
    srand(time(NULL));
    int random_number = rand() % 2;
    table[index] = random_number;
  }

  void _process(ac_int<32, false> pc, bool& isBranch)
  {
    ac_int<LOG_ENTRIES, false> index = pc.slc<LOG_ENTRIES>(2);
    isBranch                         = table[index] <= T_FINAL;
  }
};

using BranchPredictor = BitBranchPredictor<2, 4>;

#endif /* INCLUDE_BRANCHPREDICTOR_H_ */
