#include <iostream>
#include "ModuleCounter.h"

int main(int argc, char** argv)
{
    std::string path = "/Users/dillonl/Projects/SCIModuleCounter/src/";
    SCI_Module_Counter::ModuleCounter tmp;
    tmp.RetrieveFilePaths(path);
	return 0;
}