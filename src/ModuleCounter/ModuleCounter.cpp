#include "ModuleCounter.h"
#include <boost/filesystem.hpp>
using namespace SCI_Module_Counter;

	ModuleCounter::ModuleCounter()
	{
      	}
	ModuleCounter::~ModuleCounter()
	{
	}

std::vector<std::string> ModuleCounter::RetrieveAllFilePaths(std::string pathStr)
{
  std::vector<std::string> AllFolderPaths = RetrieveAllDirPaths(pathStr);
  std::vector<std::string> AllFilePaths;
    for(int i=0; i<AllFolderPaths.size(); ++i)
      {
	std::vector<std::string> tmpPaths = RetrieveFilePaths(AllFolderPaths[i]);
	std::vector<std::string> combinedVec;
	combinedVec.reserve( AllFilePaths.size() + tmpPaths.size() );
      combinedVec.insert( combinedVec.end(), AllFilePaths.begin(), AllFilePaths.end() );
      combinedVec.insert( combinedVec.end(), tmpPaths.begin(), tmpPaths.end() );
      AllFilePaths = combinedVec;
      }
    return AllFilePaths;
}

std::vector<std::string> ModuleCounter::RetrieveFilePaths(std::string pathStr)
{
  std::vector<std::string> paths;
  namespace fs = boost::filesystem;
  fs::path someDir(pathStr);
  fs::directory_iterator endIter;
  
  for(fs::directory_iterator dir_iter(someDir); dir_iter != endIter; ++dir_iter)
    {
      fs::path tmpPath = (*dir_iter);
      if(fs::extension(tmpPath).compare(".srn")==0)
      {
	std::string tmpPathString = tmpPath.string();
	paths.push_back(tmpPathString);
      }
    }
  return paths;
}

std::vector<std::string> ModuleCounter::RetrieveDirPaths(std::string pathStr)
{
  std::vector<std::string> dirPaths;
  namespace fs = boost::filesystem;
  fs::path someDir(pathStr);
  fs::directory_iterator endIter;
  
  for(fs::directory_iterator dir_iter(someDir); dir_iter != endIter; ++dir_iter)
    {
      fs::path tmpPath = (*dir_iter);
      if(fs::is_directory(tmpPath))
      {
	std::string tmpPathString = tmpPath.string();
	dirPaths.push_back(tmpPathString);
      }
    }
  return dirPaths;
}

std::vector<std::string> ModuleCounter::RetrieveAllDirPaths(std::string pathStr)
{
  std::vector<std::string> dirPaths;
  std::vector<std::string> masterDirPaths;
  dirPaths.push_back(pathStr);
  while(dirPaths.size() > 0)
    {
      
      std::string tmpPath = dirPaths.back(); 
      dirPaths.pop_back();
      masterDirPaths.push_back(tmpPath);
      std::vector<std::string> tmpPaths = RetrieveDirPaths(tmpPath);
      std::vector<std::string> combinedVec;
      combinedVec.reserve( dirPaths.size() + tmpPaths.size() );
      combinedVec.insert( combinedVec.end(), dirPaths.begin(), dirPaths.end() );
      combinedVec.insert( combinedVec.end(), tmpPaths.begin(), tmpPaths.end() );
      dirPaths = combinedVec;
                
    }
  return masterDirPaths;
}
