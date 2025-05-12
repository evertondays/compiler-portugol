#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <fstream>
#include "tokens.h"

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
    {"SENÃO", std::regex("^senão$")},
    {"STRING", std::regex("^\"([^\"\\\\]|\\\\.)*\"$")},
    {"TIPO",  std::regex("^inteiro$")},
    {"OU", std::regex("^ou$")},
    {"ATR", std::regex("^<-$")},
    {"ID",  std::regex("^[A-Za-zÀ-ÿ][A-Za-z0-9À-ÿ]*$")},
    {"SEMICOLON", std::regex("^;$")},// WE DON'T USE THIS
};

struct Token {
    std::string type;
    std::string value;
    int line;
    int column;
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
        if (token.type == "ENDLINE") {
            output_file << std::endl;
            continue;
        }

        if (token.type == "SEMICOLON") {
            continue;
        }

        output_file << token.type << "(";

        if (token_has_value(token.type)) {
            output_file << token.value << ",";
        }
        output_file << token.line << ":" << token.column << ")";
    }

    output_file.close();
    std::cout << "Tokens gravados no arquivo " << filename << std::endl;
}

int main() {
    std::string file_path = "teste.POR";
    std::ifstream inputFile(file_path);

    if (!inputFile) {
        std::cerr << "Não foi possível  abrir o arquivo.\nNome do arquivo esperado: " << file_path << std::endl;
        return 1;
    }

    auto tokens = lexer(inputFile);
    write_tokens_to_file(tokens);

    inputFile.close();
    return 0;
}