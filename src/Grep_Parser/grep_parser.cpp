#include "grep_parser.hpp"
#include <iostream>
#include <vector>
#include <cctype>

std::vector<std::string> parseExpressionPattern(std::string expression);
int doesStringContainsNumber(const std::string &s);
int doesStringContainsAlphaNumericCharacter(const std::string &s);
int doesStringContainsPositiveCharacterGroups(const std::string input_value, const std::string grp);
int doesStringContainsNegativeCharacterGroups(const std::string input_line, const std::string grp);
bool doesStringContainsMatchedPattern(const std::string input_line, const std::string v);
bool recursivelyLookForPattern(const std::string input_line, std::vector<std::string> v, int index);

GREP::GREP_PARSER::GREP_PARSER(std::string exp) { expression = exp; }

bool GREP::GREP_PARSER::match_pattern(const std::string &input_line)
{

    if (expression.length() == 1)
    {
        return input_line.find(expression) != std::string::npos;
    }
    else
    {
        if (expression == "\\d")
        {
            return doesStringContainsNumber(input_line) < 0 ? 0 : 1;
        }
        else if (expression == "\\w")
        {
            return doesStringContainsAlphaNumericCharacter(input_line) < 0 ? 0 : 1;
        }
        else if (expression[0] == '[' && expression[expression.length() - 1] == ']')
        {
            if (expression[1] == '^')
                return doesStringContainsNegativeCharacterGroups(input_line, expression) < 0 ? 0 : 1;
            return doesStringContainsPositiveCharacterGroups(input_line, expression) < 0 ? 0 : 1;
        }
        else if (expression[0] == '^')
        {
            return input_line.find(expression.substr(1, expression.length() - 1)) != 0 ? false : true;
        } else if(expression[expression.length() - 1] == '$') {
            std::string expressionString = expression.substr(0, expression.length() - 1);

            return input_line.find(expressionString) != input_line.length() - expressionString.length() ? false : true;
        }
        return doesStringContainsMatchedPattern(input_line, expression);
    }
}

std::vector<std::string> parseExpressionPattern(std::string expression)
{

    std::vector<std::string> tokens;
    std::string token{""};
    for (int i = 0; i < expression.length(); i++)
    {
        int asci_value = expression[i];
        if (asci_value == 92)
            continue;
        else if (i > 0 && expression[i - 1] == 92)
            tokens.push_back(expression.substr(i - 1, 2));
        else if (asci_value == 32)
        {
            if (token.length() > 0)
            {
                token += expression[i];
            }
            else
            {
                tokens.push_back(" ");
            }
        }
        else if (asci_value == 91)
        {
            token = "[";
        }
        else if (asci_value == 93)
        {
            token += "]";
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += expression[i];
        }
    }

    if (token.length())
        tokens.push_back(token);

    return tokens;
}

int doesStringContainsNumber(const std::string &s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (isdigit(s[i]))
            return i;
    }
    return -1;
}

int doesStringContainsAlphaNumericCharacter(const std::string &s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (isalnum(s[i]))
            return i;
    }
    return -1;
}

int doesStringContainsPositiveCharacterGroups(const std::string input_line, const std::string grp)
{
    for (int i = 1; i < grp.length() - 1; i++)
    {
        if (input_line.find(grp[i]) != std::string::npos)
            return i;
    }

    return -1;
}

int doesStringContainsNegativeCharacterGroups(const std::string input_line, const std::string grp)
{
    std::string grpstring = grp.substr(2, grp.length() - 3);
    for (int i = 0; i < input_line.length(); i++)
    {
        if (grpstring.find(input_line[i]) == std::string::npos)
            return i;
    }

    return -1;
}

bool doesStringContainsMatchedPattern(const std::string input_line, const std::string expression)
{
    auto v = parseExpressionPattern(expression);
    return recursivelyLookForPattern(input_line, v, 0);
}

bool recursivelyLookForPattern(const std::string input_line, std::vector<std::string> v, int index)
{
    if (index >= v.size())
        return true;
    else if (input_line.length() < (v.size() - index))
        return false;
    else if (index == 0)
    {
        if (v[index] == "\\d")
        {
            int nextIndex = doesStringContainsNumber(input_line);
            if (nextIndex == -1)
                return false;
            auto p = recursivelyLookForPattern(input_line.substr(nextIndex + 1), v, index + 1);
            auto q = recursivelyLookForPattern(input_line.substr(nextIndex + 1), v, index);
            if (p == true || q == true)
                return true;
            else
                return false;
        }
        else if (v[index] == "\\w")
        {
            int nextIndex = doesStringContainsAlphaNumericCharacter(input_line);
            if (nextIndex == -1)
                return false;
            auto p = recursivelyLookForPattern(input_line.substr(nextIndex + 1), v, index + 1);
            auto q = recursivelyLookForPattern(input_line.substr(nextIndex + 1), v, index);
            if (p == true || q == true)
                return true;
            else
                return false;
        }
        else
        {
            auto foundSubstringPosition = input_line.find(v[index]);
            if (foundSubstringPosition == std::string::npos)
                return false;
            else
            {
                auto p = recursivelyLookForPattern(input_line.substr(foundSubstringPosition, input_line.length() - v[index].length()), v, index + 1);
                auto q = recursivelyLookForPattern(input_line.substr(foundSubstringPosition, input_line.length() - v[index].length()), v, index);

                if (p == true || q == true)
                    return true;
                else
                    return false;
            }
        }
    }
    else
    {
        if (v[index] == "\\d")
        {
            auto x = doesStringContainsNumber(input_line);
            if (x != 0)
                return false;
            return recursivelyLookForPattern(input_line.substr(1, input_line.length() - 1), v, index + 1);
        }
        else if (v[index] == "\\w")
        {
            auto x = doesStringContainsAlphaNumericCharacter(input_line);
            if (x != 0)
                return false;
            return recursivelyLookForPattern(input_line.substr(1, input_line.length() - 1), v, index + 1);
        }
        else
        {
            auto nextString = input_line.substr(0, v[index].length());
            if (nextString != v[index])
                return false;
            return recursivelyLookForPattern(input_line.substr(nextString.length(), input_line.length() - nextString.length()), v, index + 1);
        }
    }
}
