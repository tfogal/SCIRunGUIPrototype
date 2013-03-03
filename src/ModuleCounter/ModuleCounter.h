#ifndef MODULE_COUNTER_H
#define MODULE_COUNTER_H

#include <vector>
#include <string>

namespace SCI_Module_Counter
{
class ModuleCounter
{
public:
	ModuleCounter();
	~ModuleCounter();
    
    std::vector<std::string> RetrieveFilePaths(std::string& path);
private:
};
}

#endif