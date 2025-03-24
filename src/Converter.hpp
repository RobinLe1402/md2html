#ifndef ROBINLE_MD_TO_HTML_CONVERTER
#define ROBINLE_MD_TO_HTML_CONVERTER





#include "LineBuffer.hpp"
#include "Settings.hpp"

#include <string>
#include <vector>



class Converter final
{
private: // variables

    std::istream &m_oInput;
    std::ostream &m_oOutput;

    bool m_bErrorMessage = false;


private: // methods

    void processBlock(LineBuffer &buffer);

    
public: // methods

    Converter(Settings &settings);
    Converter(const Converter &) = delete;
    Converter(Converter &&) = default;
    ~Converter() = default;

    bool run();

    bool errorMessagePrinted() const { return m_bErrorMessage; }

};





#endif // ROBINLE_MD_TO_HTML_CONVERTER