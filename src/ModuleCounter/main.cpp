#include <iostream>
#include "ModuleCounter.h"

int main(int argc, char** argv) // enables us to pass in argument from command line
{
  std::string pathStr = "/Users/ajwaller/Development/scirun4/trunk/SCIRun/src/nets";  // hard coded path in anyway
  SCI_Module_Counter::ModuleCounter tmp;
    
  std::vector<std::string> filePaths = tmp.RetrieveAllFilePaths(pathStr);
   
  for(int i=0; i<filePaths.size(); ++i)
  {
    std::cout << filePaths.at(i) << std::endl;
  }
   
    return 0;
}
