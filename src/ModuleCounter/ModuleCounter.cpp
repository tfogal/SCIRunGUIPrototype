#include "ModuleCounter.h"
#include <boost/filesystem.hpp>
using namespace SCI_Module_Counter;

	ModuleCounter::ModuleCounter()
	{
      	}
	ModuleCounter::~ModuleCounter()
	{
	}
    
    std::vector<std::string> ModuleCounter::RetrieveFilePaths(std::string pathStr) // path is the starting directory
    {
        std::vector<std::string> paths; // paths will contain all ".srn" paths in path and subdirs under path
        namespace fs = boost::filesystem; // namespace syntax sugar
        fs::path someDir(pathStr); // someDir is a path object for dir path (starting directory)
        fs::directory_iterator endIter; // a null directory_iterator that represents 1 past the end of a directory.  by convention with no param is null of right type
        
        for(fs::directory_iterator dir_iter(someDir); dir_iter != endIter; ++dir_iter) // dir_iter represents an iterator for each item (fs::path) in a given directory
        {
            fs::path tmpPath = (*dir_iter); //tmpPath a derefenced dir_iter which is a child file/folder of path
            if(fs::extension(tmpPath).compare(".srn") == 0) //checking the extension for .srn files.  the 0 means success in this case  = .srn file
            {
                std::string tmpPathString = tmpPath.string(); // full path string of tmpPath
                paths.push_back(tmpPathString); //pushing onto end of paths vector
            }
        }
       
        //Once we are here, paths should have all .srn files in the path directory (not subdirs yet)
        //the for loop below searches folders that are children of path
	for (fs::directory_iterator path_iter(someDir); path_iter != endIter; ++path_iter) // dir_iter represents an iterator for each item (fs::path) in a given directory
        {
	    fs::path tmpPath = (*path_iter); //tmpPath a derefenced dir_iter which is a child file/folder of path
            if (fs::is_directory(tmpPath)) //check if is tmpPath is a directory
            {
	      std::string childFolderPathString = tmpPath.string(); //childFolderPathString is the full path string of tmpPath
                std::vector<std::string> tmpPaths = this->RetrieveFilePaths(childFolderPathString); // call RetrieveFilePaths on childFolderPathString and store the results (all .srn files in childFolderPathString and all subdirs of childFolderPathString) into tmpPaths
                    //combines the paths vector with tmpPaths vector
                std::vector<std::string> combinedVec;
                combinedVec.reserve( paths.size() + tmpPaths.size() ); // preallocate memory
                combinedVec.insert( combinedVec.end(), paths.begin(), paths.end() );
                combinedVec.insert( combinedVec.end(), tmpPaths.begin(), tmpPaths.end() );
                paths = combinedVec;
                //at this point paths contains path + tmpPaths
		}
	}
        return paths;
    }