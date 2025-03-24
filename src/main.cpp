#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "CommandLineParams.hpp"
#include "Converter.hpp"
#include "Settings.hpp"



void PrintHelp(const char *szArgv0);

int main(int argc, char* argv[])
{
    CommandLineParams cmd(argc, argv);

    if (!cmd.valid())
    {
        if (!cmd.errorMessagePrinted())
            std::fprintf(stderr, "Invalid syntax.\n");

        return 1;
    }



    if (cmd.helpRequested())
    {
        PrintHelp(argv[0]);
        return 0;
    }



    Settings settings;

    std::shared_ptr<std::istream> sp_input;
    std::shared_ptr<std::ostream> sp_output;


    // input path specified --> read file
    if (cmd.inputPath())
    {
        sp_input = std::make_unique<std::ifstream>(cmd.inputPath());
        settings.input = sp_input.get();
        if (!*settings.input)
        {
            std::fprintf(stderr, "Failed to open file \"%s\" for reading.\n", cmd.inputPath());
            return 1;
        }
    }

    // no input path specified --> read stdin
    else
        settings.input = &std::cin;


    // output path specified --> write file
    if (cmd.outputPath())
    {
        sp_output = std::make_unique<std::ofstream>(cmd.outputPath());
        settings.output = sp_output.get();
        if (!*settings.input)
        {
            std::fprintf(stderr, "Failed to open file \"%s\" for writing.\n", cmd.outputPath());
            return 1;
        }
    }

    // no input path specified --> read stdin
    else
        settings.output = &std::cout;


    
    Converter cvt(settings);
    if (!cvt.run())
    {
        if (!cvt.errorMessagePrinted())
            std::fprintf(stderr, "Unknown error.\n");

        return 1;
    }



    return 0;
}


void PrintHelp(const char *szArgv0)
{
    std::printf(
        "Usage: %s [OPTIONS]\n"
        "\n"
        "Options:\n"
        "  -i, --input-file <path>   Specify the input file.\n"
        "                             If this parameter is not present, standard input is read.\n"
        "  -o, --output-file <path>  Specify the output file.\n"
        "                             If this parameter is not present, the result is written to\n"
        "                             standard output.\n"
        "  -h, --help                 Show this message and exit.\n"
        "\n"
        "Note that all input is assumed to be UTF-8 and all output will be UTF-8 too.\n",
        szArgv0
    );
}
