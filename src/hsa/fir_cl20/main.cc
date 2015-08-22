/*
 * Hetero-mark
 *
 * Copyright (c) 2015 Northeastern University
 * All rights reserved.
 *
 * Developed by:
 *   Northeastern University Computer Architecture Research (NUCAR) Group
 *   Northeastern University
 *   http://www.ece.neu.edu/groups/nucar/
 *
 * Author: Yifan Sun (yifansun@coe.neu.edu)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal with the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 *   Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimers.
 *
 *   Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimers in the 
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the names of NUCAR, Northeastern University, nor the names of 
 *   its contributors may be used to endorse or promote products derived 
 *   from this Software without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS WITH THE SOFTWARE.
 */

#include "src/common/Benchmark/BenchmarkRunner.h"
#include "src/common/Timer/TimeMeasurement.h"
#include "src/common/Timer/TimeMeasurementImpl.h"
#include "src/common/CommandLineOption/CommandLineOption.h"
#include "src/hsa/fir_cl20/FirBenchmark.h"

int main(int argc, const char **argv) {
  // Setup command line option
  CommandLineOption commandLineOption(
    "====== Hetero-Mark FIR Benchmarks (HSA mode) ======",
    "This benchmarks runs Finite Impulse Response (FIR) filter.");
  commandLineOption.addArgument("Help", "bool", "false",
      "-h", "--help", "Dump help information");
  commandLineOption.addArgument("NumData", "integer", "1024",
      "-n", "--num-data",
      "Number of data elements in each data block");
  commandLineOption.addArgument("NumBlock", "integer", "1024",
      "-b", "--num-block",
      "Number of data blocks, each data block is process in one kernel "
      "launching");

  commandLineOption.parse(argc, argv);
  if (commandLineOption.getArgumentValue("Help")->asBool()) {
    commandLineOption.help();
    return 0;
  }
  uint32_t numData = commandLineOption.getArgumentValue("NumData")
    ->asUInt32();
  uint32_t numBlock = commandLineOption.getArgumentValue("NumBlock")
    ->asUInt32();

  // Create and run benchmarks
  std::unique_ptr<FirBenchmark> benchmark(new FirBenchmark());
  benchmark->setNumData(numData);
  benchmark->setNumBlocks(numBlock);
  std::unique_ptr<TimeMeasurement> timer(new TimeMeasurementImpl());
  BenchmarkRunner runner(benchmark.get(), timer.get());
  runner.run();
  runner.summarize();
}
