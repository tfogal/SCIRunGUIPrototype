// if def prevents multiple includes of this code

#ifndef MODULE_COUNTER_H
#define MODULE_COUNTER_H

#include <vector>
#include <string>

// decided not to write code in global name space to prevent possible name collisions in code
namespace SCI_Module_Counter
{
class ModuleCounter
{
public:
	ModuleCounter();
	~ModuleCounter();
    
    std::vector<std::string> RetrieveFilePaths(std::string pathStr);
    std::vector<std::string> RetrieveDirPaths(std::string pathStr);
    std::vector<std::string> RetrieveAllDirPaths(std::string pathStr);
    std::vector<std::string> RetrieveAllFilePaths(std::string pathStr);
};
}

#endif
