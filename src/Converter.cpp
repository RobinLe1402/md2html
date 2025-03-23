#include "Converter.hpp"

Converter::Converter(Settings &settings) :
    m_oInput(*settings.input),
    m_oOutput(*settings.output)
{ }

bool Converter::run()
{
    std::fprintf(stderr, "Converter::run() is not implemented yet!\n");
    m_bErrorMessage = true;

    return false; // TODO
}
