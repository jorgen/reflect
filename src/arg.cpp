#include "arg.h"

#include <iostream>
#include <stdio.h>

#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

enum optionIndex {
	UNKNOWN,
	HELP,
	SYSROOT,
	SRCROOT,
	INCLUDE,
	DEFINE,
        OUTFILE
};

const option::Descriptor usage[] =
{
	{UNKNOWN,       0,  "", "",                Arg::unknown,                 "USAGE: reflect [options] file\n\n Options:" },
	{HELP,          0, "?", "help",            option::Arg::None,            "  --help, -? \tPrint usage and exit." },
	{INCLUDE,       0, "I", "",                Arg::requiresExistingDir,     "  -I        \tDirectory for adding directory to include path." },
	{DEFINE,        0, "D", "",                option::Arg::Optional,        "  -D        \tAdd compiler define"},
	{SYSROOT,       0, "" , "sysroot",         Arg::requiresExistingDir,     "  --sysroot \tDirectory for sysroot."},
	{SRCROOT,       0, "" , "srcroot",         Arg::requiresExistingDir,     "  --srcroot \tDirectory for src root of project"},
	{OUTFILE,       0, "o" , "outfile",        Arg::requiresWritableFile,    "  -o --outfile \tOutput file. Default is stdout"},

	{UNKNOWN,       0, "" ,  ""   ,            option::Arg::None,            "\nExamples:\v"
	 "reflect -I some/include main.cpp"},
	{0,0,0,0,0,0}
};

static bool isFile(const char *file)
{
    struct stat stat_buf;
    stat(file, &stat_buf);

    return S_ISREG(stat_buf.st_mode);
}

Configuration Arg::getConfig(int argc, char **argv)
{
	argc-=(argc>0); argv+=(argc>0);

	Configuration conf;
        conf.out_file = stdout;
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
        if (parser.nonOptionsCount() < 1) {
            fprintf(stderr, "Please provide an input file\n");
            option::printUsage(std::cerr, usage);
            exit(1);
        }

        if (parser.nonOptionsCount() > 1) {
            fprintf(stderr, "Please provide only one input file\n");
            option::printUsage(std::cerr, usage);
            exit(1);
        }

        conf.file = parser.nonOption(0);
        if (!isFile(conf.file.c_str())) {
            fprintf(stderr, "Input file %s does not exist\n", conf.file.c_str());
            option::printUsage(std::cerr, usage);
            exit(1);
        }

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
                        break;
                case OUTFILE:
                        conf.out_file = fopen(opt.arg, "w+");
                        break;
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

option::ArgStatus Arg::requiresWritableFile(const option::Option &option, bool msg)
{
    FILE *file = fopen(option.arg, "w+");

    if (!file)
    {
        if (msg) {
            fprintf (stderr, "Error opening write file: %s\n",strerror(errno));
            printError("Option '", option, "' requires a file that can be opened\n");
            return option::ARG_ILLEGAL;
        }
    }
    return option::ARG_OK;
}

option::ArgStatus Arg::unknown(const option::Option &option, bool msg)
{
    if (msg)
        printError("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
}
