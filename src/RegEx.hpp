#ifndef ROBINLE_MD_TO_HTML_REGEX
#define ROBINLE_MD_TO_HTML_REGEX





#include <regex>



namespace RegEx
{

    const auto TRIM = std::regex(R"RegEx(\s*([^\s]+)\s*)RegEx", std::regex::optimize);



    const auto CODEBLOCK_INDENTED = std::regex(R"RegEx(^\s{4}(.+)$)RegEx", std::regex::optimize);

    const auto BLOCKQUOTE = std::regex(R"RegEx(^ *> ?(.*)$)RegEx", std::regex::optimize);

    const auto CODEBLOCK_BACKTICKS_OPEN =
        std::regex(R"RegEx(^ *```([a-z]*)$)RegEx", std::regex::optimize | std::regex::icase);
    const auto CODEBLOCK_BACKTICKS_CLOSE =
        std::regex(R"RegEx(^ {0,3}```$)RegEx", std::regex::optimize);

    const auto TABLE_LINE_HEAD = std::regex(R"RegEx(^\s*\|(.*\|)\s*$)RegEx",  std::regex::optimize);
    const auto TABLE_LINE_HEAD_SEPARATOR =
        std::regex(R"RegEx(^\s*\|(\s*:?-+:?\s*\|)+$)RegEx", std::regex::optimize);
    const auto TABLE_LINE = std::regex(R"RegEx(^\s*\|(.*\|)?\s*$)RegEx", std::regex::optimize);
    const auto TABLE_ITEM = std::regex(R"RegEx(\s*([^|]*?)\s*\|)RegEx",  std::regex::optimize);
    const auto TABLE_HEAD_SEPARATOR = std::regex(R"RegEx((:?)-+(:?))RegEx", std::regex::optimize);

}





#endif // ROBINLE_MD_TO_HTML_REGEX