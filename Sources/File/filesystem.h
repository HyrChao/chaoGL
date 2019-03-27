#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <chaoGL.h>

#include <string>
#include <cstdlib>


class FileSystem
{
    
public:
    static char const * app_root;
private:
	typedef std::string(*Builder) (const std::string& path);

public:
	static std::string getPath(const std::string& path)
	{
		static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
		//static std::string root = (givenRoot != nullptr ? givenRoot : "");
		return (*pathBuilder)(path); 
	}
//    static std::string GetDirectory (const std::string& path)
//    {
//        size_t found = path.find_last_of("/\\");
//        return(path.substr(0, found));
//    }

private:
	static std::string const & getRoot()
	{
        static char const * envRoot = getenv("LOGL_ROOT_PATH");
        static char const * givenRoot = (app_root != nullptr ? app_root : logl_root);
        static std::string root = (givenRoot != nullptr ? givenRoot : envRoot);
		return root;
	}

	//static std::string(*foo (std::string const &)) getPathBuilder()
	static Builder getPathBuilder()
	{
		if (getRoot() != "")
			return &FileSystem::getPathRelativeRoot;
		else
			return &FileSystem::getPathRelativeBinary;
	}

	static std::string getPathRelativeRoot(const std::string& path)
	{
		return getRoot() + std::string("/") + path;
	}

	static std::string getPathRelativeBinary(const std::string& path)
	{
		return "../../../" + path;
	}

};

// FILESYSTEM_H
#endif
