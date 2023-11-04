#include <iostream>
#include <set>
#include <map>
#include "grammar.hpp"
#include "parsing.hpp"
#include "readers.hpp"
#include <vector>
#include <format>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include "firstfollow.hpp"


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
    std::ifstream inputFile{argv[2]};
    if (!inputFile.is_open()) {
        std::perror("The input file");
        return EXIT_FAILURE;
    }

    grammar::Grammar grammar{readers::ReadGrammar(grammarFile)};
    int ruleIndex{1};
    for (const auto &[ruleInput, ruleOutput]: grammar) {
        std::cout << ruleIndex << ") " << ruleInput << " - " << ruleOutput << '\n';
        ++ruleIndex;
    }

    std::map<char, std::set<char>> first = firstfollow::firstK(grammar);
    firstfollow::displayResult(first, "First k");
    std::cout << '\n';

    std::map<char, std::set<char>> follow = firstfollow::followK(grammar, first);
    firstfollow::displayResult(follow, "Follow k");

    std::vector<std::set<char>> intermediateParsingTable{parsing::BuildIntermediateTable(grammar)};
    std::cout << "\nIntermediate parsing table:\n";
    for (int i = 0; i < intermediateParsingTable.size(); ++i) {
        std::cout << i + 1 << ") {";
        std::copy(intermediateParsingTable[i].begin(), intermediateParsingTable[i].end(),
                  std::ostream_iterator<char>{std::cout, ", "});
        std::cout << "}\n";
    }

    std::cout << "\nInput tokens:\n";
    std::vector<char> tokens{readers::ReadTokens(inputFile)};
    std::copy(tokens.cbegin(), tokens.cend(), std::ostream_iterator<char>{std::cout, " "});

    parsing::syntacticAnalysis(grammar, tokens);
    return EXIT_SUCCESS;
}
