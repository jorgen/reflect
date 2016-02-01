#include "arg.h"

#include <iostream>
#include <stdio.h>

#include <sys/stat.h>
#include <errno.h>

enum optionIndex {
	UNKNOWN,
	HELP,
	SYSROOT,
	SRCROOT,
	INCLUDE,
	DEFINE
};

const option::Descriptor usage[] =
{
	{UNKNOWN,       0,  "", "",                Arg::unknown,                 "USAGE: reflect [options] file\n\n Options:" },
	{HELP,          0, "?", "help",            option::Arg::None,            "  --help, -? \tPrint usage and exit." },
	{INCLUDE,       0, "I", "",                Arg::requiresExistingDir,     "  -I        \tDirectory for adding directory to include path." },
	{DEFINE,        0, "D", "",                option::Arg::Optional,        "  -D        \tAdd compiler define"},
	{SYSROOT,       0, "" , "sysroot",         Arg::requiresExistingDir,     "  --sysroot \tDirectory for sysroot."},
	{SRCROOT,       0, "" , "srcroot",         Arg::requiresExistingDir,     "  --srcroot \tDirectory for src root of project"},

	{UNKNOWN,       0, "" ,  ""   ,            option::Arg::None,            "\nExamples:\v"
	 "reflect -I some/include main.cpp"},
	{0,0,0,0,0,0}
};

Configuration Arg::getConfig(int argc, char **argv)
{
	argc-=(argc>0); argv+=(argc>0);

	Configuration conf;
	option::Stats  stats(true, usage, argc, argv);
	std::vector<option::Option> options(stats.options_max);
	std::vector<option::Option> buffer(stats.buffer_max);
	option::Parser parser(true, usage, argc, argv, options.data(), buffer.data());

	if (parser.error()) {
		exit(1);
	}

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		exit(0);
	}
	for (size_t i = 0; i < parser.nonOptionsCount(); i++) {
		conf.files.push_back(parser.nonOption(i));
	}
	if (!Arg::allExistingFiles(conf.files))
		exit(0);

	for (int i = 0; i < parser.optionsCount(); ++i)
	{
		option::Option& opt = buffer[i];
		switch (opt.index())
		{
		case HELP:
			// not possible, because handled further above and exits the program
			break;
		case SYSROOT:
			conf.sysroot = opt.arg;
			break;
		case SRCROOT:
			conf.srcroot = opt.arg;
			break;
		case INCLUDE:
			conf.include_dirs.push_back(opt.arg);
			break;
		case DEFINE:
			conf.defines.push_back(opt.arg);
		}
	}

	if (!conf.srcroot.size()) {
		char p[512];
		getcwd(p, sizeof p);
		conf.srcroot = p;
	}

	return conf;
}

void Arg::printError(const char* msg1, const option::Option& opt, const char* msg2)
{
  fprintf(stderr, "%s", msg1);
  fwrite(opt.name, opt.namelen, sizeof(char), stderr);
  fprintf(stderr, "%s", msg2);

}

static bool isFile(const char *file)
{
    struct stat stat_buf;
    stat(file, &stat_buf);

    return S_ISREG(stat_buf.st_mode);
}

static bool isDir(const char *file)
{
    struct stat stat_buf;
    stat(file, &stat_buf);

    return S_ISDIR(stat_buf.st_mode);
}

option::ArgStatus Arg::requiresExistingDir(const option::Option &option, bool msg)
{
    if (isDir(option.arg))
        return option::ARG_OK;

    if (msg)
        printError("Option '", option, "' requires an existing path\n");

    return option::ARG_ILLEGAL;
}

bool Arg::allExistingFiles(const std::vector<std::string> &files)
{
    for (size_t i = 0; i < files.size(); i++) {
        if (!isFile(files.at(i).c_str())) {
            fprintf(stderr, "Not all files specified files exists: %s\n", files.at(i).c_str());
            return false;
        }
    }
    return true;
}

option::ArgStatus Arg::unknown(const option::Option &option, bool msg)
{
    if (msg)
        printError("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
}
