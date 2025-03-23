#ifndef ROBINLE_MD_TO_HTML_REGEX
#define ROBINLE_MD_TO_HTML_REGEX





#include <regex>



namespace RegEx
{

    const auto CODEBLOCK_INDENTED = std::regex(R"RegEx(^\s{4}(.+)$)RegEx", std::regex::optimize);

}





#endif // ROBINLE_MD_TO_HTML_REGEX