#include "LineBuffer.hpp"

#include <iostream>
#include <memory>
#include <string>

bool LineBuffer::fillBuffer(bool bPeek) const
{
    const int iMinSize = 1 + (int)bPeek;

    if (m_oBuffer.size() >= iMinSize)
        return true;

    if (m_bFullyRead)
        return false;

    switch (m_oBuffer.size())
    {
    case 0:
    {
        std::string sTemp;

        m_bFullyRead = !readLine(sTemp);
        if (m_bFullyRead)
            return false;

        m_oBuffer.push_back(std::move(sTemp));
        if (!bPeek)
            return true;
    }

    [[fallthrough]];
    case 1:
    {
        std::string sTemp;

        m_bFullyRead = !readLine(sTemp);
        if (m_bFullyRead)
            return false;

        m_oBuffer.push_back(std::move(sTemp));
        return true;
    }


    default: // should never happen, just written down to avoid compiler warnings
        return true;
    }
}

LineBuffer::LineBuffer(const std::vector<std::string> &buffer) :
    m_oBuffer(buffer),
    m_bFullyRead(true)
{
}

LineBuffer::LineBuffer(std::vector<std::string> &&buffer) :
    m_oBuffer(std::move(buffer)),
    m_bFullyRead(true)
{}

const std::string &LineBuffer::current() const
{
    if (!fillBuffer(false))
        return "";

    return m_oBuffer[0];
}

bool LineBuffer::eof() const
{
    return m_oBuffer.size() == 0 && !fillBuffer(false);
}

bool LineBuffer::hasNext() const
{
    return fillBuffer(true);
}

std::string LineBuffer::popCurrent()
{
    if (!fillBuffer(false))
        throw std::exception();

    std::string sResult = std::move(m_oBuffer[0]);
    m_oBuffer.erase(m_oBuffer.begin());
    return sResult;
}

bool LineBuffer::next()
{
    const bool result = fillBuffer(true);
    if (!m_oBuffer.empty())
        m_oBuffer.erase(m_oBuffer.begin());
    return result;
}

const std::string &LineBuffer::peekNext()
{
    if (!fillBuffer(true))
        return "";

    return m_oBuffer[1];
}



bool LineBuffer_Prebuffered::readLine(std::string &dest) const
{
    // this function will never be called, as bFullyRead is set to true by the constructor of
    // LineBuffer used by this derived class.
    return false;
}

LineBuffer_Prebuffered::LineBuffer_Prebuffered(const std::vector<std::string> &buffer) :
    LineBuffer(buffer)
{

}

LineBuffer_Prebuffered::LineBuffer_Prebuffered(std::vector<std::string> &&buffer) :
    LineBuffer(std::move(buffer))
{

}



bool LineBuffer_InputStream::readLine(std::string &dest) const
{
    return (bool)std::getline(m_oInput, dest);
}

LineBuffer_InputStream::LineBuffer_InputStream(std::istream &input) : m_oInput(input)
{

}
