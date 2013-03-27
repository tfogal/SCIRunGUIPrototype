/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2012 Scientific Computing and Imaging Institute,
 University of Utah.
 
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Testing/Tools/ModuleCounter.h>

// Step 1: single file data collection
// step 2: directory traversal, applying step 1 and collecting results
// step 3: data analysis


TEST(ModuleCounterStatistics, PrintNetworkFiles)
{
std::string pathStr = "/Users/ajwaller/Development/scirun4/trunk/SCIRun/src/nets";  // add option later for user specified path
SCI_Module_Counter::ModuleCounter tmp;

std::vector<std::string> filePaths = tmp.RetrieveFilePaths(pathStr);

for(int i=0; i<filePaths.size(); ++i)
{
  std::cout << filePaths.at(i) << std::endl;
  }

}

TEST(ModuleCounterStatistics, MoveDirectory)
{


}
