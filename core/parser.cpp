#include "parser.h"
#include "tokens.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>

struct Instruction {
    std::vector<std::string> tokens;
    std::string make;
};

struct ReduceResult {
  bool success = false;
  std::string reduce;
  int quantity_of_reduce_tokens = 0;
};

const std::vector<Instruction> GrammarQueue;
const std::vector<Instruction> InstructionsList;

std::vector<Instruction> init_instructions() {
    std::vector<Instruction> instructions;

    Instruction instruction;
    instruction.tokens = {"MATH_OPERATION", "OPMAIS", "NUMINT"};
    instruction.make = "MATH_OPERATION";
    instructions.push_back(instruction);

    instruction.tokens = {"NUMINT"};
    instruction.make = "MATH_OPERATION";
    instructions.push_back(instruction);

    return instructions;
}

ReduceResult try_reduce(int token_index, const std::vector<Token>& queue, const std::vector<Instruction>& instructions) {
	ReduceResult result;

	for (const auto& instruction : instructions) {
		bool match = true;
		int tokens_to_check = instruction.tokens.size();

		for (int i = 0; i < tokens_to_check; ++i) {
			if (instruction.tokens[i] != queue[token_index + i].type) {
				match = false;
				break;
			}
		}

		if (match) {
			result.success = true;
			result.reduce = instruction.make;
			result.quantity_of_reduce_tokens = tokens_to_check;
			return result;
		}
	}

	return result;
}

void parser(std::vector<Token>& tokens) {
	auto instructions = init_instructions();
	std::vector<Token> queue;

	while (!tokens.empty()) {
		queue.push_back(tokens.front());
		tokens.erase(tokens.begin());

		bool reduced = false;

		for (int i = 0; i < queue.size(); ++i) {
			auto reduce_result = try_reduce(i, queue, instructions);

			if (reduce_result.success) {
				Token reduced_token;
				reduced_token.type = reduce_result.reduce;
				queue.erase(queue.begin() + i, queue.begin() + i + reduce_result.quantity_of_reduce_tokens);
				queue.insert(queue.begin() + i, reduced_token);
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
