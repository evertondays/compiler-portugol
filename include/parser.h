#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

std::vector<Token> parser(std::vector<Token> tokens);
bool validate_parser(std::vector<Token> queue);

#endif  //PARSER_H
