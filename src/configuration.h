#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include <stdio.h>

struct Configuration
{
	std::string root_src_dir;
	std::string sysroot;
	std::string srcroot;
	std::string file;
        std::string out_file_name;
        std::string tmp_file_name;
	std::vector<std::string> include_dirs;
	std::vector<std::string> defines;
        FILE *out_file;
};

#endif //CONFIGURATION_H
