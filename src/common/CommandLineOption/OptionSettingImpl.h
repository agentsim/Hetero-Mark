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

#ifndef SRC_COMMON_COMMANDLINEOPTION_OPTIONSETTINGIMPL_H_
#define SRC_COMMON_COMMANDLINEOPTION_OPTIONSETTINGIMPL_H_

#include <map>
#include <string>

#include "src/common/CommandLineOption/OptionSetting.h"

class OptionSettingImpl : public OptionSetting {
 public:
  /**
   * The iterator for arguments
   */
  class Iterator : public OptionSetting::Iterator {
   public:
    Iterator(
      std::map<std::string, std::unique_ptr<Argument>>::iterator begin,
      std::map<std::string, std::unique_ptr<Argument>>::iterator end) :
      begin(begin),
      iterator(begin),
      end(end) {}
    bool hasNext() override;
    Argument *next() override;

   private:
    std::map<std::string, std::unique_ptr<Argument>>::iterator begin;
    std::map<std::string, std::unique_ptr<Argument>>::iterator iterator;
    std::map<std::string, std::unique_ptr<Argument>>::iterator end;
  };

  /**
   * Constructor
   */
  OptionSettingImpl(const char *name, const char *description) :
    name(name),
    description(description) {}

  /**
   * Add an argument
   */
  void addArgument(std::unique_ptr<Argument> argument) override;

  /**
   * Get the argument iterator
   */
  std::unique_ptr<OptionSetting::Iterator> getIterator() override;

  const std::string getProgramName() override {
    return name;
  }

  const std::string getProgramDescription() override {
    return description;
  }

 protected:
  std::map<std::string, std::unique_ptr<Argument>> arguments;
  std::string name;
  std::string description;
};

#endif  // SRC_COMMON_COMMANDLINEOPTION_OPTIONSETTINGIMPL_H_
