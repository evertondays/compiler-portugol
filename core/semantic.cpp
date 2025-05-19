#include "semantic.h"
#include "tokens.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>

bool validate_semantic(std::vector<Token>& tokens) {
	std::unordered_map<std::string, Token> variables;

	for (size_t i = 0; i < tokens.size(); ++i) {
		auto& token = tokens[i];
		if (token.type != "ID") {
			continue;
		}

		if (tokens[i - 1].type == "DOISPONTOS") {
			if (variables.find(token.value) != variables.end()) {
				// Variavel já declarada
				auto& variable = variables[token.value];
				std::cout << ANSI_COLOR_RED << std::endl << "Variável já declarada!" << std::endl << "A variável '" << token.value << "' já foi declarda na linha: " << variable.line << std::endl;
				std::cout << "Linha: " << token.line << " Coluna: " << token.column << std::endl << ANSI_COLOR_RESET;
				return false;
			} else {
				variables[token.value] = token;
			}
		} else {
			if (variables.find(token.value) != variables.end()) {
				continue;
			}
			// Variavel não declarada
			std::cout << ANSI_COLOR_RED << std::endl << "Variável não declarada!" << std::endl << "A variável '" << token.value << "' está tenando ser acessada porém ainda não foi inicializada." << std::endl;
			std::cout << "Linha: " << token.line << " Coluna: " << token.column << std::endl << ANSI_COLOR_RESET;
			return false;
		}
	}

	return true;
}