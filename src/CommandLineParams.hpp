#ifndef ROBINLE_MD_TO_HTML_COMMANDLINEPARAMS
#define ROBINLE_MD_TO_HTML_COMMANDLINEPARAMS





#include <map>
#include <string>


class CommandLineParams final
{
private: // variables

    bool m_bValid = true;
    bool m_bErrorMessage = false;

    bool m_bHelp = false;
    const char *m_szInputPath = nullptr;
    const char *m_szOutputPath = nullptr;


public: // methods

    CommandLineParams(int argc, char* argv[]);
    ~CommandLineParams() = default;

    bool valid() const { return m_bValid; }
    bool errorMessagePrinted() const { return m_bErrorMessage; }


    bool helpRequested() const { return m_bHelp; }

    const char *inputPath() const { return m_szInputPath; }
    const char *outputPath() const { return m_szOutputPath; }

};





#endif // ROBINLE_MD_TO_HTML_COMMANDLINEPARAMS