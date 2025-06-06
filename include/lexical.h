#ifndef LEXICAL_H
#define LEXICAL_H

std::vector<Token> lexer(std::ifstream& source_file);
void write_tokens_to_file(const std::vector<Token>& tokens);
void clear_tokens(std::vector<Token>& tokens);
bool validate_tokens(std::vector<Token>& tokens);

#endif  //LEXICAL_H
