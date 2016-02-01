#ifndef ARG_H
#define ARG_H

#include "optionparser.h"
#include "configuration.h"
#include <string>
#include <vector>

class Arg
{
public:
	static Configuration getConfig(int argc, char **argv);
    static void printError(const char* msg1, const option::Option& opt, const char* msg2);
    static option::ArgStatus requiresExistingDir(const option::Option &option, bool msg);
    static option::ArgStatus unknown(const option::Option &option, bool msg);
    static bool allExistingFiles(const std::vector<std::string> &files);
};

#endif
