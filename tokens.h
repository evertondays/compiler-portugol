#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <regex>
#include <vector>

struct TokenPattern {
    std::string name;
    std::regex pattern;
};


// extern const std::vector<TokenPattern> token_patterns;

#endif