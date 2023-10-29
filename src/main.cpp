#include <cstdlib>
#include <iostream>
#include <fstream>
#include <format>
#include <iterator>
#include "grammar.hpp"
#include "readers.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << std::format("Usage: {} <path to the grammar file> <path to the input file>", argv[0]);
        return EXIT_FAILURE;
    }
    std::ifstream grammarFile{argv[1]};
    if (!grammarFile.is_open()) {
        std::perror("The grammar file");
        return EXIT_FAILURE;
    }
    std::ifstream inputFile{argv[2]};;
    if (!inputFile.is_open()) {
        std::perror("The input file");
        return EXIT_FAILURE;
    }

    grammar::Grammar grammar{readers::ReadGrammar(grammarFile)};
    for (const auto &[ruleInput, ruleOutput] : grammar) {
        std::cout << ruleInput << " → " << ruleOutput << '\n';
    }

    std::vector<char> tokens{readers::ReadTokens(inputFile)};
    std::copy(tokens.cbegin(), tokens.cend(), std::ostream_iterator<char>{std::cout, " "});

    return EXIT_SUCCESS;
}
