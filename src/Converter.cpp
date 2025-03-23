#include "Converter.hpp"

Converter::Converter(Settings &settings) :
    m_oInput(*settings.input),
    m_oOutput(*settings.output)
{ }

bool Converter::run()
{
    std::string sLine;
    while (std::getline(m_oInput, sLine))
    {
        m_oLines.push_back(sLine);
    }

    // TODO: actually process the data
    for (const auto &sLine : m_oLines)
    {
        m_oOutput << sLine << "\n";
    }

    return true;
}
