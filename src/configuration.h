#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>

struct Configuration
{
	std::string root_src_dir;
	std::string sysroot;
	std::string srcroot;
	std::vector<std::string> files;
	std::vector<std::string> include_dirs;
	std::vector<std::string> defines;
};

#endif //CONFIGURATION_H
