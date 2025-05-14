// Compiler options
// core/lexical.cpp core/parser.cpp -o main

#include "core/tokens.h"
#include "core/lexical.h"
#include "core/parser.h"

#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::string file_path = "teste.POR";
    std::ifstream inputFile(file_path);

    if (!inputFile) {
        std::cerr << "Não foi possível  abrir o arquivo.\nNome do arquivo esperado: " << file_path << std::endl;
        return 1;
    }

    auto tokens = lexer(inputFile);
    write_tokens_to_file(tokens);

    parser(tokens);

    inputFile.close();
    return 0;
}