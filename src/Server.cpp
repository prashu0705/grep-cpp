#include <iostream>
#include <string>
#include <stack>
#include <cctype> // For isdigit, isalnum

// Function to check if the input line contains any digit
bool matchdigit(const std::string& input_line) {
    for (char ch : input_line) {
        if (std::isdigit(ch)) return true;
    }
    return false;
}

// Function to check if the input line contains any alphanumeric character
bool matchalphanumeric(const std::string& input_line) {
    for (char ch : input_line) {
        if (std::isalnum(ch)) return true;
    }
    return false;
}

// Function to handle positive match group
bool positiveMatchGroup(const std::string& input_line, const std::string& pattern, int start, int end) {
    std::stack<char> s;
    std::stack<std::pair<char, char>> s_pair;

    if (end >= pattern.size()) end = pattern.size();
    int idx = start;

    while (idx < end) {
        if (idx != end - 1 && pattern[idx] == '-') {
            idx++;
            char temp = s.top();
            s.pop();
            s_pair.push({ temp, pattern[idx] });
        } else {
            s.push(pattern[idx]);
        }
        idx++;
    }

    while (!s.empty()) {
        char temp = s.top();
        s.pop();
        if (input_line.find(temp) != std::string::npos) return true;
    }

    while (!s_pair.empty()) {
        auto [temp1, temp2] = s_pair.top();
        s_pair.pop();
        for (char ch = temp1; ch <= temp2; ch++) {
            if (input_line.find(ch) != std::string::npos) return true;
        }
    }

    return false;
}

// Function to handle negative match group
bool negitiveMatchGroup(const std::string& input_line, const std::string& pattern, int start, int end) {
    std::stack<char> s;
    std::stack<std::pair<char, char>> s_pair;

    if (end >= pattern.length()) end = pattern.length();
    int idx = start + 1;

    while (idx < end - 1) {
        if (idx != end - 1 && pattern[idx] == '-') {
            idx++;
            char temp = s.top();
            s.pop();
            s_pair.push({ temp, pattern[idx] });
        } else {
            s.push(pattern[idx]);
        }
        idx++;
    }

    while (!s.empty()) {
        char temp = s.top();
        s.pop();
        if (input_line.find(temp) != std::string::npos) return false;
    }

    while (!s_pair.empty()) {
        auto [temp1, temp2] = s_pair.top();
        s_pair.pop();
        for (char ch = temp1; ch <= temp2; ch++) {
            if (input_line.find(ch) != std::string::npos) return false;
        }
    }

    return true;
}

// Function to match the input line with the pattern
bool match(const std::string& input_line, const std::string& pattern) {
    int i = 0;
    while (i < input_line.size()) {
        int j = 0;
        int temp = i;
        while (j < pattern.size() && temp < input_line.size()) {
            if (pattern[j] == '\\') {
                j++;
                if (j < pattern.size()) {
                    if (pattern[j] == 'd') {
                        if (!std::isdigit(input_line[temp])) break;
                        else temp++;
                    } else if (pattern[j] == 'w') {
                        if (!std::isalnum(input_line[temp])) break;
                        else temp++;
                    } else if (pattern[j] == '[') {
                        int start = j;
                        while (j < pattern.size() && pattern[j] != ']') j++;
                        if (j < pattern.size() && pattern[j] == ']') {
                            if (pattern[start + 1] == '^') {
                                return negitiveMatchGroup(input_line, pattern, start, j + 1);
                            } else {
                                return positiveMatchGroup(input_line, pattern, start, j + 1);
                            }
                        }
                        break;
                    }
                } else break;
            } else {
                if (input_line[temp] != pattern[j]) break;
                else temp++;
            }
            j++;
        }
        if (j == pattern.size()) return true;
        i++;
    }
    return false;
}

// Main function to test the pattern matching
bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    } else if (pattern[0] == '\\' && pattern.length() == 2) {
        switch (pattern[1]) {
        case 'd':
            return matchdigit(input_line);
        case 'w':
            return matchalphanumeric(input_line);
        default:
            throw std::runtime_error("Unhandled pattern " + pattern);
        }
    } else if (pattern[0] == '[' && pattern[pattern.length() - 1] == ']') {
        if (pattern[1] == '^') {
            return negitiveMatchGroup(input_line, pattern, 0, pattern.size());
        } else {
            return positiveMatchGroup(input_line, pattern, 0, pattern.size());
        }
    } else if (pattern.length() > 1) {
        return match(input_line, pattern);
    } else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }
    std::string flag = argv[1];
    std::string pattern = argv[2];
    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    std::string input_line;
    std::getline(std::cin, input_line);

    try {
        if (match_pattern(input_line, pattern)) {
            return 0;
        } else {
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}





