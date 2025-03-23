#include "CommandLineParams.hpp"

#include <filesystem>
#include <set>

namespace
{
    enum class Parameter
    {
        Help,
        InputFile,
        OutputFile
    };

    const auto oFLAGS = std::map<char, Parameter>(
    {
        { 'h', Parameter::Help       },
        { 'i', Parameter::InputFile  },
        { 'o', Parameter::OutputFile }
    });

    const auto oPARAMS = std::map<std::string, Parameter>(
    {
        { "help",        Parameter::Help       },
        { "input-file",  Parameter::InputFile  },
        { "output-file", Parameter::OutputFile }
    });



    namespace fs = std::filesystem;


    bool CheckFile(const char *szPath)
    {
        if (!fs::exists(szPath))
        {
            std::fprintf(stderr, "File \"%s\" doesn't exist.\n", szPath);
            return false;
        }

        if (!fs::is_regular_file(szPath))
        {
            std::fprintf(stderr, "\"%s\" is not a file.\n", szPath);
            return false;
        }

        return true;
    }

}

CommandLineParams::CommandLineParams(int argc, char* argv[])
{
    std::set<Parameter> params;
    for (unsigned iArg = 1; iArg < (unsigned)argc; ++iArg)
    {
        params.clear();

        const char *sz = argv[iArg];
        if (*sz != '-')
        {
            m_bValid = false;
            return;
        }

        ++sz;

        // "--" --> single long parameter
        if (*sz == '-')
        {
            ++sz;
            const auto it = oPARAMS.find(sz);

            if (it == oPARAMS.end())
            {
                std::fprintf(stderr, "Unknown parameter \"%s\".\n", sz);

                m_bErrorMessage = true;
                m_bValid        = false;
                return;
            }
            else
                params.insert(it->second);
        }

        // "-" --> flag(s)
        else
        {
            while (*sz)
            {
                const auto it = oFLAGS.find(*sz);
                if (it == oFLAGS.end())
                {
                    std::fprintf(stderr, "Unknown flag \"%c\".\n", *sz);

                    m_bErrorMessage = true;
                    m_bValid        = false;
                    return;
                }

                params.insert(it->second);

                ++sz;
            }
        }



        // evaluate parameters
        unsigned iSkippedParams = 0;
        for (const auto param : params)
        {
            switch (param)
            {
            case Parameter::Help:
                m_bHelp = true;
                break;

            case Parameter::InputFile:
                if (iArg + 1 == argc)
                {
                    std::fprintf(stderr, "Missing input file path.\n");

                    m_bErrorMessage = true;
                    m_bValid        = false;
                    return;
                }
                else
                {
                    m_szInputPath = argv[iArg + 1];
                    iSkippedParams = 1;

                    if (!CheckFile(m_szInputPath))
                    {
                        m_bErrorMessage = true;
                        m_bValid        = false;
                        return;
                    }
                }
                break;

            case Parameter::OutputFile:
                if (iArg + 1 == argc)
                {
                    std::fprintf(stderr, "Missing output file path.\n");

                    m_bErrorMessage = true;
                    m_bValid        = false;
                    return;
                }
                else
                {
                    m_szOutputPath = argv[iArg + 1];
                    iSkippedParams = 1;

                    if (!CheckFile(m_szOutputPath))
                    {
                        m_bErrorMessage = true;
                        m_bValid        = false;
                        return;
                    }
                }
                break;
            }
        }

        iArg += iSkippedParams;
    }


    if (m_szInputPath && m_szOutputPath)
    {
        try
        {
            if (fs::equivalent(m_szInputPath, m_szOutputPath))
            {
                std::fprintf(stderr, "The input file can't be the same as the output file.\n");

                m_bErrorMessage = true;
                m_bValid        = false;
                return;
            }
        }
        catch (const fs::filesystem_error &e)
        {
            std::fprintf(stderr, "Error evaluating the given paths:\n%s\n", e.what());
            
            m_bValid        = false;
            m_bErrorMessage = true;
            return;
        }
    }

}
