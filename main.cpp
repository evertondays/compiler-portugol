// Compiler options
// core/lexical.cpp core/parser.cpp core/semantic.cpp -o main

#include "core/tokens.h"
#include "core/lexical.h"
#include "core/parser.h"
#include "core/semantic.h"

#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::cout << "***********************" << std::endl;
    std::cout << "** Portugol Compiler **" << std::endl;
    std::cout << "***********************" << std::endl << std::endl;

    std::string file_path = "teste.POR";
    std::ifstream inputFile(file_path);

    if (!inputFile) {
        std::cerr << "Não foi possível  abrir o arquivo.\nNome do arquivo esperado: " << file_path << std::endl;
        return 1;
    }

    std::cout << "Iniciando análise léxica . . ." << std::endl;
    auto tokens = lexer(inputFile);
    inputFile.close();
    write_tokens_to_file(tokens);
    clear_tokens(tokens);
    if (!validate_tokens(tokens)) {
        return 1;
    }
    std::cout << "Análise léxica concluída. (1/3)" << std::endl;

    std::cout << "Iniciando análise sintática . . ." << std::endl;
    auto final_parser_queue = parser(tokens);
	if (!validate_parser(final_parser_queue)) {
		return 1;
	}
	std::cout << "Análise sintática concluída. (2/3)" << std::endl;

	std::cout << "Iniciando análise semântica . . ." << std::endl;
	if (!validate_semantic(tokens)) {
		return 1;
	}
	std::cout << "Análise semântica concluída. (3/3)" << std::endl;

    return 0;
}