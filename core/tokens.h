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

#endif