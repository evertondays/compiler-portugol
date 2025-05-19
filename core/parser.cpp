#include "parser.h"
#include "tokens.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>

struct Instruction {
    std::vector<std::string> tokens;
    std::string make;
	bool is_final = false;
};

struct ReduceResult {
  bool success = false;
  std::string reduce;
  int quantity_of_reduce_tokens = 0;
  bool is_final = false;
};

const std::vector<Instruction> GrammarQueue;
const std::vector<Instruction> InstructionsList;

std::vector<Instruction> init_instructions() {
    std::vector<Instruction> instructions;

    Instruction instruction;

	// leia / escreva
	instruction.tokens = {"ESCREVA", "PARAB", "NUMINT", "PARFE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"ESCREVA", "PARAB", "ID", "PARFE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"ESCREVA", "PARAB", "STRING", "PARFE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"LEIA", "PARAB", "ID", "PARFE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	// Operações matématicas
    instruction.tokens = {"MATH_OPERATION_OR_NUM", "OPMAIS", "MATH_OPERATION_OR_NUM"};
    instruction.make = "MATH_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"MATH_OPERATION_OR_NUM", "OPMENOS", "MATH_OPERATION_OR_NUM"};
    instruction.make = "MATH_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"MATH_OPERATION_OR_NUM", "OPDIVI", "MATH_OPERATION_OR_NUM"};
    instruction.make = "MATH_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"MATH_OPERATION_OR_NUM", "OPMULTI", "MATH_OPERATION_OR_NUM"};
    instruction.make = "MATH_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"PARAB", "MATH_OPERATION", "PARFE"};
    instruction.make = "MATH_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	// Atribuições
	instruction.tokens = {"ID", "ATR", "MATH_OPERATION"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"ID", "ATR", "ID"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	// Expressões logicas
	instruction.tokens = {"ID_OR_NUM", "LOGIC_TOKEN", "ID_OR_NUM"};
	instruction.make = "LOGIC_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"ID_OR_NUM", "LOGIC_TOKEN", "LOGIC_OPERATION"};
	instruction.make = "LOGIC_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"LOGIC_OPERATION", "LOGIC_TOKEN", "LOGIC_OPERATION"};
	instruction.make = "LOGIC_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"PARAB", "LOGIC_OPERATION", "PARFE"};
	instruction.make = "LOGIC_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	instruction.tokens = {"NAO", "LOGIC_OPERATION"};
	instruction.make = "LOGIC_OPERATION";
	instruction.is_final = false;
    instructions.push_back(instruction);

	// Declaração de variaveis
	//  instruction.tokens = {"TIPO", "ATR", "MATH_OPERATION"};
	//	instruction.is_final = true;
	//  instructions.push_back(instruction);

	// se / para
	instruction.tokens = {"SE", "LOGIC_OPERATION", "ENTAO", "SENAO", "FIMSE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"SE", "LOGIC_OPERATION", "ENTAO", "FIMSE"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"PARA", "ID_OR_NUM", "ATE", "ID_OR_NUM", "PASSO", "ID_OR_NUM", "FIMPARA"};
	instruction.is_final = true;
    instructions.push_back(instruction);

	instruction.tokens = {"PARA", "ID_OR_NUM", "ATE", "ID_OR_NUM", "FIMPARA"};
	instruction.is_final = true;
    instructions.push_back(instruction);

    return instructions;
}

bool tokens_are_equal(std::string queue_token, std::string instruction_token) {
	if (instruction_token == "ID_OR_NUM") {
		return queue_token == "ID" || queue_token == "NUMINT" || queue_token == "MATH_OPERATION";
	}

	if (instruction_token == "LOGIC_TOKEN") {
		return queue_token == "LOGIGUAL" || queue_token == "LOGMAIOR" || queue_token == "LOGMAIORIGUAL"
		|| queue_token == "LOGMAIORIGUAL" || queue_token == "LOGMENOR" || queue_token == "LOGMENORIGUAL"
		|| queue_token == "LOGDIFF" || queue_token == "OU" || queue_token == "E";
	}

	if (instruction_token == "MATH_OPERATION_OR_NUM") {
		return queue_token == "MATH_OPERATION" || queue_token == "NUMINT";
	}

	return queue_token == instruction_token;
}

ReduceResult try_reduce(int token_index, const std::vector<Token>& queue, const std::vector<Instruction>& instructions) {
	ReduceResult result;

	for (const auto& instruction : instructions) {
		bool match = true;
		int tokens_to_check = instruction.tokens.size();

		if (token_index - tokens_to_check + 1 < 0) continue;

		for (int i = 0; i < tokens_to_check; ++i) {
			if (!tokens_are_equal(queue[token_index - i].type, instruction.tokens[i])) {
				match = false;
				break;
			}
		}

		if (match) {
			result.success = true;
			result.reduce = instruction.make;
			result.quantity_of_reduce_tokens = tokens_to_check;
			result.is_final = instruction.is_final;
			return result;
		}
	}

	return result;
}

void parser(std::vector<Token>& tokens) {
	auto instructions = init_instructions();
	std::vector<Token> queue;
	bool reduced = false;

	while (!tokens.empty() || reduced) {
		if (!tokens.empty()) {
			queue.push_back(tokens.back());
			tokens.pop_back();
		}

		reduced = false;

		for (int i = queue.size() - 1; i >= 0; --i) {
			auto reduce_result = try_reduce(i, queue, instructions);

			if (reduce_result.success) {
				Token reduced_token;
				reduced_token.type = reduce_result.reduce;

				queue.erase(queue.begin() + (i - reduce_result.quantity_of_reduce_tokens + 1), queue.begin() + i + 1);

				if (!reduce_result.is_final) {
					queue.insert(queue.begin() + (i - reduce_result.quantity_of_reduce_tokens + 1), reduced_token);
				}

				reduced = true;
				break;
			}
		}

		if (!reduced && tokens.empty()) {
			break;
		}
	}

	std::cout << "Queue final:" << std::endl;
	for (const auto& item : queue) {
		std::cout << item.type << std::endl;
	}
}
