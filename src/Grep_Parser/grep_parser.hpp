#if !defined(GREP_PARSER_HPP)
#define GREP_PARSER_HPP

#include <string>

namespace GREP
{

    class GREP_PARSER
    {
    private:
        std::string expression;

    public:
        GREP_PARSER(std::string exp);

        bool match_pattern(const std::string &input_line);
    };

};

#endif // GREP_PARSER_HPP
