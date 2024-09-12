#ifndef PATTERN_MATCHER_H
#define PATTERN_MATCHER_H

#include <string>
#include <vector> // Include this to use std::vector

// Function to split pattern alternatives separated by '|'
std::vector<std::string> split_alternatives(const std::string& pattern);

// Function to check if a character is in a given string
bool contains_any(const std::string& text, const std::string& chars);

// Function to check if a string starts with a prefix
bool starts_with(const std::string& text, const std::string& prefix);

// Function to check if a string ends with a suffix
bool ends_with(const std::string& text, const std::string& suffix);

// Recursive function to match a text with a pattern
bool match_pattern_rec(const std::string& text, const std::string& pattern);

// Main function to match a pattern with optional start (^) and end ($) anchors
bool match_pattern(const std::string& text, const std::string& pattern);

#endif // PATTERN_MATCHER_H

