#include "tokens.h"
#include "lexical.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>

const std::vector<TokenPattern> token_patterns = {
    {"ATE", std::regex("^até$")},
    {"E", std::regex("^e$")},
    {"ENTAO", std::regex("^então$")},
    {"ESCREVA", std::regex("^escreva$")},
    {"FIMPARA", std::regex("^fim_para$")},
    {"FIMSE", std::regex("^fim_se$")},
    {"LEIA", std::regex("^leia$")},
    {"LOGDIFF", std::regex("^<>$")},
    {"LOGIGUAL", std::regex("^=$")},
    {"LOGMAIOR", std::regex("^>$")},
    {"LOGMENOR", std::regex("^<$")},
    {"LOGMAIORIGUAL", std::regex("^>=$")},
    {"LOGMENORIGUAL", std::regex("^<=$")},
    {"NAO", std::regex("^não$")},
    {"NUMINT", std::regex("^[0-9]+$")},
    {"OPDIVI",  std::regex("^/$")},
    {"OPMAIS",  std::regex("^\\+$")},
    {"OPMENOS", std::regex("^\\-$")},
    {"OPMULTI", std::regex("^\\*$")},
    {"PARA", std::regex("^para$")},
    {"PARAB", std::regex("^\\($")},
    {"PARFE", std::regex("^\\)$")},
    {"PASSO", std::regex("^passo$")},
    {"SE", std::regex("^se$")},
    {"SENAO", std::regex("^senão$")},
    {"STRING", std::regex("^\"([^\"\\\\]|\\\\.)*\"$")},
    {"TIPO",  std::regex("^inteiro$")},
    {"OU", std::regex("^ou$")},
    {"ATR", std::regex("^<-$")},
    {"ID",  std::regex("^[A-Za-zÀ-ÿ][A-Za-z0-9À-ÿ]*$")},
    {"SEMICOLON", std::regex("^;$")},// WE DON'T USE THIS
};

Token generate_token(std::string type, std::string value, int line, int column) {
    Token token;

    token.value = value;
    token.type = type;
    token.line = line;
    token.column = column;

    return token;
}

Token identify_token(std::string word, int line, int column) {
    Token token;
    std::string token_type = "UNKNOWN";

    for (const auto& token_pattern : token_patterns) {
        if (std::regex_match(word, token_pattern.pattern)) {
            token_type = token_pattern.name;
            break;
        }
    }

    return generate_token(token_type, word, line, column);
}

void flush_word(std::vector<Token>& tokens, std::string& word, int line_count, int column_count) {
    if (!word.empty()) {
        tokens.push_back(identify_token(word, line_count, column_count));
        word.clear();
    }
}

bool token_has_value(const std::string& type) {
    return type == "NUMINT" || type == "ID" || type == "UNKNOWN" || type == "STRING";
}

std::vector<Token> lexer(std::ifstream& source_file) {
    std::vector<Token> tokens;

    std::string line;
    int line_count = 1;
    bool antesessor_is_less_than = false;

    while (std::getline(source_file, line)) {
        int column_count = 0;
        std::string word;
        bool is_string = false;

        for (char character : line) {
            column_count++;

            if (is_string) {
                word += character;
                if (character == '"') {
                    flush_word(tokens, word, line_count, column_count);
                    is_string = false;
                }
                continue;
            }

            if (character == '"') {
                word += character;
                is_string = true;
                continue;
            }

            if (character == ' ') {
                flush_word(tokens, word, line_count, column_count);
                tokens.push_back(generate_token("SPACE", word, line_count, column_count));
                continue;
            }

            if (character == '\t') {
                flush_word(tokens, word, line_count, column_count);
                tokens.push_back(generate_token("TAB", word, line_count, column_count));
                continue;
            }

            switch (character) {
                case '/':
                case '+':
                case '*':
                case '(':
                case ')':
                case ';':
                flush_word(tokens, word, line_count, column_count);
                    word += character;
                    tokens.push_back(identify_token(word, line_count, column_count));
                    word.clear();
                    continue;
            }

            if (antesessor_is_less_than) {
                antesessor_is_less_than = false;

                if (!word.empty()) {
                    word.pop_back();
                    flush_word(tokens, word, line_count, column_count);
                }

                if (character == '>' || character == '=' || character == '-') {
                    word = "<";
                    word += character;
                    flush_word(tokens, word, line_count, column_count);
                } else {
                    word = "<";
                    flush_word(tokens, word, line_count, column_count);
                }

                continue;
            }

            if (character == '<') {
                antesessor_is_less_than = true;
            }

            word += character;
        }

        flush_word(tokens, word, line_count, column_count);
        tokens.push_back(generate_token("ENDLINE", "", line_count, column_count));
        line_count++;
    }

    return tokens;
}

void write_tokens_to_file(const std::vector<Token>& tokens) {
    std::string filename = "build/tokens.OBJ";
    std::ofstream output_file(filename);

    if (!output_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return;
    }

    for (const auto& token : tokens) {
        if (token.type == "SEMICOLON" || token.type == "SPACE") {
            continue;
        }

        if (token.type == "ENDLINE") {
            output_file << std::endl;
            continue;
        }

        if (token.type == "TAB") {
            output_file << "    ";
            continue;
        }

        output_file << token.type << " ";
    }

    output_file.close();
    std::cout << "Tokens gravados no arquivo " << filename << std::endl;
}

bool validate_tokens(std::vector<Token>& tokens) {
    for (const Token& token : tokens) {
        if (token.type == "UNKNOWN") {
            std::cout << ANSI_COLOR_RED << std::endl << "Token inválido!" << std::endl << "A palavra '" << token.value << "' não é reconhecido pela linguagem." << std::endl;
            std::cout << "Linha: " << token.line << " Coluna: " << token.column << std::endl << ANSI_COLOR_RESET;
            return false;
        }
    }
    return true;
}

void clear_tokens(std::vector<Token>& tokens) {
    auto new_end = std::remove_if(tokens.begin(), tokens.end(),
        [](const Token& token) {
            return token.type == "ENDLINE"
                || token.type == "TAB"
                || token.type == "SPACE";
        });

    tokens.erase(new_end, tokens.end());
}