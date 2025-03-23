#ifndef ROBINLE_MD_TO_HTML_CONVERTER
#define ROBINLE_MD_TO_HTML_CONVERTER





#include "Settings.hpp"

#include <string>
#include <vector>



class Converter final
{
private: // variables

    std::istream &m_oInput;
    std::ostream &m_oOutput;

    bool m_bErrorMessage = false;

    std::vector<std::string> m_oLines;

    
public: // methods

    Converter(Settings &settings);
    Converter(const Converter &) = delete;
    Converter(Converter &&) = default;
    ~Converter() = default;

    bool run();

    bool errorMessagePrinted() const { return m_bErrorMessage; }

};





#endif // ROBINLE_MD_TO_HTML_CONVERTER