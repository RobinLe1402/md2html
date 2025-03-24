#include "Converter.hpp"

#include "RegEx.hpp"

namespace
{

    bool EmptyLine(const std::string &s)
    {
        for (const auto c : s)
        {
            switch (c)
            {
            case ' ':
            case '\t':
                break;

            default:
                return false;
            }
        }
        
        return true;
    }

}

Converter::Converter(Settings &settings) :
    m_oInput(*settings.input),
    m_oOutput(*settings.output)
{ }

void Converter::processBlock(const std::vector<std::string> &oTextBlock)
{
    size_t iLine = 0;

    std::smatch oMatch;

    while (iLine < oTextBlock.size())
    {
        // skip empty lines at the start of any block
        if (EmptyLine(oTextBlock[iLine]))
        {
            ++iLine;
            continue;
        }


        const auto &sLine = oTextBlock[iLine];

        if (std::regex_search(sLine, oMatch, RegEx::CODEBLOCK_INDENTED))
        {
            std::vector<std::string> oCodeBlock;
            oCodeBlock.push_back(oMatch[1].str());

            size_t iFirstEmpty = 1;
            while (++iLine < oTextBlock.size())
            {
                const auto &sLine = oTextBlock[iLine];

                if (std::regex_search(sLine, oMatch, RegEx::CODEBLOCK_INDENTED))
                {
                    oCodeBlock.push_back(oMatch[1].str());
                    iFirstEmpty = oCodeBlock.size();
                }
                else if (EmptyLine(sLine))
                    oCodeBlock.push_back("");
                else
                    break;
            }

            oCodeBlock.erase(oCodeBlock.begin() + iFirstEmpty, oCodeBlock.end());

            m_oOutput << "<pre>";
            for (size_t i = 0; i + 1 < oCodeBlock.size(); ++i)
            {
                m_oOutput << oCodeBlock[i] << '\n';
            }
            m_oOutput << oCodeBlock[oCodeBlock.size() - 1] << "</pre>";
        }

        else if (std::regex_search(sLine, oMatch, RegEx::BLOCKQUOTE))
        {
            m_oOutput << "<blockquote>";

            std::vector<std::string> m_oBlockquoteContent;
            m_oBlockquoteContent.push_back(oMatch[1].str());

            while (++iLine < oTextBlock.size())
            {
                const auto &sLine = oTextBlock[iLine];

                if (std::regex_search(sLine, oMatch, RegEx::BLOCKQUOTE))
                    m_oBlockquoteContent.push_back(oMatch[1].str());
                else if (std::regex_search(sLine, oMatch, RegEx::TRIM))
                    m_oBlockquoteContent.push_back(oMatch[1].str());
                else
                    break;
            }

            processBlock(m_oBlockquoteContent);

            m_oOutput << "</blockquote>";
        }

        else if (std::regex_search(sLine, oMatch, RegEx::CODEBLOCK_BACKTICKS_OPEN))
        {
            m_oOutput << "<pre";
            if (oMatch[2].length() != 0)
                m_oOutput << " lang=\"" << oMatch[2].str() << "\"";
            m_oOutput << ">";

            std::vector<std::string> oCodeBlock;

            while (++iLine < oTextBlock.size())
            {
                const auto &sLine = oTextBlock[iLine];

                if (std::regex_match(sLine, RegEx::CODEBLOCK_BACKTICKS_CLOSE))
                    break;

                oCodeBlock.push_back(sLine);
            }

            if (oCodeBlock.size() > 0)
            {
                for (size_t i = 0; i + 1 < oCodeBlock.size(); ++i)
                {
                    m_oOutput << oCodeBlock[i] << '\n';
                }
                m_oOutput << oCodeBlock[oCodeBlock.size() - 1];
            }
            m_oOutput << "</pre>";
        }



        else
        {
            m_oOutput << sLine << '\n';
            ++iLine;
        }
    }
}

bool Converter::run()
{
    std::string sLine;
    std::vector<std::string> oLines;

    while (std::getline(m_oInput, sLine))
    {
        oLines.push_back(sLine);
    }

    processBlock(oLines);

    return true;
}
