#include <iostream>
#include <string>

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if(pattern == "\\d") {
              return input_line.find_first_of("123456890") != std::string::npos;
    } 
    else if (pattern == "\\w") {  // Matches any alphanumeric character or underscore
        for (const auto &c : input_line) {
            if (std::isalnum(c) || c == '_') {
                return true;
            }
        }
        return false;

      }
    else if (pattern.at(0) == '[' && pattern.at(pattern.length() - 1) == ']') {
    // Check for negative character group
    if (pattern.length() > 2 && pattern.at(1) == '^') {
        for (const auto &c : input_line) {
            bool is_match = false;
            for (const auto &l : pattern.substr(2, pattern.length() - 3)) {
                if (c == l) {
                    is_match = true;
                    break;
                }
            }
            if (!is_match) {
                return true; // Found a character not in the negative group
            }
        }
        return false; // All characters are in the negative group
    } else {
        // Original positive character group logic
        for (const auto &l : pattern.substr(1, pattern.length() - 2)) {
            if (input_line.find(l) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
}
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here" << std::endl;

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
