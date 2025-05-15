#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <regex>
#include <vector>

struct Token {
    std::string type;
    std::string value;
    int line;
    int column;
};

struct TokenPattern {
    std::string name;
    std::regex pattern;
};

extern const std::vector<TokenPattern> token_patterns;

const std::string ANSI_COLOR_RED = "\033[31m";
const std::string ANSI_COLOR_RESET = "\033[0m";

#endif