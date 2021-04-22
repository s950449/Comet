/** Copyright 2021 INRIA, Université de Rennes 1 and ENS Rennes
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*       http://www.apache.org/licenses/LICENSE-2.0
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/



#ifndef __BASIC_SIMULATOR_H__
#define __BASIC_SIMULATOR_H__

#include <vector>
#include "ac_int.h"
#include "simulator.h"

#define DRAM_SIZE ((size_t)1 << 26)
#define STACK_INIT (DRAM_SIZE - 0x1000)

class BasicSimulator : public Simulator {
  unsigned heapAddress;

  // Signature address when doing compliance tests
  unsigned int begin_signature, end_signature;

  std::vector<ac_int<32, false> > mem;

  FILE* inputFile;
  FILE* outputFile;
  FILE* traceFile;
  FILE* signatureFile;

public:
  BasicSimulator(std::string binaryFile, std::vector<std::string>,
                 std::string inFile, std::string outFile,
                 std::string tFile, std::string sFile);
  ~BasicSimulator();

protected:
  void printCycle();
  void printEnd();
  void extend(){};
  void printCoreReg(const char* strTemp);

  // Functions for memory accesses
  void stb(ac_int<32, false> addr, ac_int<8, true> value);
  void sth(ac_int<32, false> addr, ac_int<16, true> value);
  void stw(ac_int<32, false> addr, ac_int<32, true> value);
  void std(ac_int<32, false> addr, ac_int<64, true> value);

  ac_int<8, true> ldb(ac_int<32, false> addr);
  ac_int<16, true> ldh(ac_int<32, false> addr);
  ac_int<32, true> ldw(ac_int<32, false> addr);
  ac_int<32, true> ldd(ac_int<32, false> addr);

  // Functions for solving syscalls
  void solveSyscall();

  ac_int<32, true> doRead(ac_int<32, false> file, ac_int<32, false> bufferAddr, ac_int<32, false> size);
  ac_int<32, true> doWrite(ac_int<32, false> file, ac_int<32, false> bufferAddr, ac_int<32, false> size);
  ac_int<32, true> doOpen(ac_int<32, false> name, ac_int<32, false> flags, ac_int<32, false> mode);
  ac_int<32, true> doOpenat(ac_int<32, false> dir, ac_int<32, false> name, ac_int<32, false> flags,
                            ac_int<32, false> mode);
  ac_int<32, true> doLseek(ac_int<32, false> file, ac_int<32, false> ptr, ac_int<32, false> dir);
  ac_int<32, true> doClose(ac_int<32, false> file);
  ac_int<32, true> doStat(ac_int<32, false> filename, ac_int<32, false> ptr);
  ac_int<32, true> doSbrk(ac_int<32, false> value);
  ac_int<32, true> doGettimeofday(ac_int<32, false> timeValPtr);
  ac_int<32, true> doUnlink(ac_int<32, false> path);
  ac_int<32, true> doFstat(ac_int<32, false> file, ac_int<32, false> stataddr);

private:
  void setByte(unsigned, ac_int<8, true>);
  void readElf(const char*);
  void pushArgsOnStack(const std::vector<std::string>);
  void openFiles(std::string inFile, std::string outFile, std::string tFile, std::string sFile);
};

#endif // __BASIC_SIMULATOR_H__
