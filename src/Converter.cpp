#include "Converter.hpp"

#include "RegEx.hpp"


// reference:
// https://spec-md.com/

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


    bool IsTableHead(const std::string &sLine1, const std::string &sLine2)
    {
        std::smatch oMatch;

        // check for general syntax
        if (!std::regex_search(sLine1, oMatch, RegEx::TABLE_LINE_HEAD) ||
            !std::regex_search(sLine2, RegEx::TABLE_LINE_HEAD_SEPARATOR)
        )
            return false;

        const auto iColCount_Line1 = std::distance(
            std::sregex_iterator(sLine1.begin(), sLine1.end(), RegEx::TABLE_ITEM),
            std::sregex_iterator()
        );

        const auto iColCount_Line2 = std::distance(
            std::sregex_iterator(sLine2.begin(), sLine2.end(), RegEx::TABLE_ITEM),
            std::sregex_iterator()
        );

        return iColCount_Line1 == iColCount_Line2;
    }

    void ParseTableLine(const std::string &sLine, std::vector<std::string> &oCells)
    {
        oCells.clear();

        auto words_begin = std::sregex_iterator(sLine.begin(), sLine.end(), RegEx::TABLE_ITEM);
        auto words_end = std::sregex_iterator();

        // Iterate over all matches
        for (std::sregex_iterator it = words_begin; it != words_end; ++it)
        {
            oCells.push_back((*it)[1].str());
        }
    }

}

Converter::Converter(Settings &settings) :
    m_oInput(*settings.input),
    m_oOutput(*settings.output)
{ }

void Converter::processBlock(LineBuffer &buffer)
{
    std::smatch oMatch;

    while (!buffer.eof())
    {
        // skip empty lines at the start of any block
        if (EmptyLine(buffer.current()))
        {
            buffer.next();
            continue;
        }


        const std::string sLine = buffer.current();

        if (std::regex_search(sLine, oMatch, RegEx::CODEBLOCK_INDENTED))
        {
            std::vector<std::string> oCodeBlock;
            oCodeBlock.push_back(oMatch[1].str());

            size_t iFirstEmpty = 1;
            while (buffer.next())
            {
                const auto &sLine = buffer.current();

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

            std::vector<std::string> oBlockquoteContent;
            oBlockquoteContent.push_back(oMatch[1].str());

            while (buffer.next())
            {
                const auto &sLine = buffer.current();

                if (std::regex_search(sLine, oMatch, RegEx::BLOCKQUOTE))
                    oBlockquoteContent.push_back(oMatch[1].str());
                else if (std::regex_search(sLine, oMatch, RegEx::TRIM))
                    oBlockquoteContent.push_back(oMatch[1].str());
                else
                    break;
            }

            LineBuffer_Prebuffered buf(std::move(oBlockquoteContent));
            processBlock(buf);

            m_oOutput << "</blockquote>";
        }

        else if (std::regex_search(sLine, oMatch, RegEx::CODEBLOCK_BACKTICKS_OPEN))
        {
            m_oOutput << "<pre";
            if (oMatch[1].length() != 0)
                m_oOutput << " lang=\"" << oMatch[2].str() << "\"";
            m_oOutput << ">";

            std::vector<std::string> oCodeBlock;

            while (buffer.next())
            {
                const auto &sLine = buffer.current();

                if (std::regex_match(sLine, RegEx::CODEBLOCK_BACKTICKS_CLOSE))
                {
                    buffer.next(); // skip the trailing ``` line
                    break;
                }

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

        else if (buffer.hasNext() && IsTableHead(sLine, buffer.peekNext()))
        {
            std::vector<std::string> oCells;

            std::vector<std::string> oDummyBlock;
            oDummyBlock.resize(1);

            // parse the separator line first, as it dictates the alignment
            std::regex_search(buffer.peekNext(), oMatch, RegEx::TABLE_LINE);
            ParseTableLine(oMatch[1].str(), oCells);
            const size_t iColumnCount = oCells.size();
            std::vector<std::string> oAlignmentStrings;
            oAlignmentStrings.resize(iColumnCount);
            for (size_t iCol = 0; iCol < iColumnCount; ++iCol)
            {
                std::regex_search(oCells[iCol], oMatch, RegEx::TABLE_HEAD_SEPARATOR);
                const int iAlignFlags =
                    1 * (oMatch[1].length() == 1) +
                    2 * (oMatch[2].length() == 1);

                std::string &sDest = oAlignmentStrings[iCol];

                switch (iAlignFlags)
                {
                case 0: // -
                    break;

                case 1: // :-
                    sDest = " align=\"left\"";
                    break;

                case 2: // -:
                    sDest = " align=\"right\"";
                    break;

                case 3: // :-:
                    sDest = " align=\"center\"";
                    break;
                }
            }


            m_oOutput << "<table><thead><tr>";
            std::regex_search(sLine, oMatch, RegEx::TABLE_LINE);
            ParseTableLine(oMatch[1].str(), oCells);
            for (size_t iCol = 0; iCol < iColumnCount; ++iCol)
            {
                m_oOutput << "<th" << oAlignmentStrings[iCol] << ">";
                oDummyBlock[0] = oCells[iCol];

                LineBuffer_Prebuffered buf(oDummyBlock);
                processBlock(buf);
                m_oOutput << "</th>";
            }
            m_oOutput << "</tr></thead>";

            buffer.next();

            const bool bTBody = buffer.hasNext() &&
                                std::regex_search(buffer.peekNext(), RegEx::TABLE_LINE);
            
            if (bTBody)
                m_oOutput << "<tbody>";
            while (buffer.next())
            {
                if (!std::regex_search(buffer.current(), oMatch, RegEx::TABLE_LINE))
                    break;

                ParseTableLine(oMatch[1].str(), oCells);
                oCells.resize(iColumnCount); // trim/expand to given column count

                m_oOutput << "<tr>";
                for (size_t iCol = 0; iCol < iColumnCount; ++iCol)
                {
                    m_oOutput << "<td" << oAlignmentStrings[iCol] << ">";
                    oDummyBlock[0] = oCells[iCol];

                    LineBuffer_Prebuffered buf(oDummyBlock);
                    processBlock(buf);
                    m_oOutput << "</td>";
                }
                m_oOutput << "</tr>";
            }
            if (bTBody)
                m_oOutput << "</tbody>";




            m_oOutput << "</table>";
        }



        else
        {
            m_oOutput << sLine << '\n';
            buffer.next();
        }
    }
}

bool Converter::run()
{
    LineBuffer_InputStream buffer(m_oInput);
    processBlock(buffer);

    return true;
}
