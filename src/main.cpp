#include <iostream>
#include "grammar.hpp"
#include "readers.hpp"
#include <vector>
char epsilon = 'e';

bool isTerminal(char a){
    if((islower(a))) return true;
    return false;
}

bool isInVector(char symbolToFind, const std::vector<char>& charVector) {

    for (char c : charVector) {
        if (c == symbolToFind) {
            return true;
        }
    }
    return false;
}

std::vector<char> getNonterminals(const grammar::Grammar& g) {

    std::vector<char> nonTerminals;
    for (const auto& rule : g) {
        char c = rule.first;
        if (!isInVector(c, nonTerminals)) {
            nonTerminals.push_back(c);
        }
    }
    return nonTerminals;
}

std::vector<char> getFirst_k(std::vector<std::pair<char, std::vector<char>>> first_kTable, char nonterm) {

    for (const auto& first_k : first_kTable) {
        char c = first_k.first;
        if(c == nonterm) return first_k.second;
    }

    return std::vector<char>{};
}

//.!.   εつ▄█▀█●
//8====o


std::vector<std::pair<char, std::vector<char>>> first_k(const grammar::Grammar& g){

    std::vector<std::pair<char, std::vector<char>>> table;
    std::vector<char> nonTerminals = getNonterminals(g);

    for (const char c : nonTerminals) {
        table.push_back(std::make_pair(c, std::vector<char>{}));
    }

    bool notChanged;

    do {

        notChanged = true;
        for (auto& column : table) {
            char nonTerminal = column.first;

            for (const auto& rule : g) {

                char leftSideRule = rule.first;
                if (leftSideRule != nonTerminal) continue;
                auto rightSideRule = rule.second.getSymbols();

                bool continueFlag = false;
                for(int i = 0; i < rightSideRule.size(); i++){

                    if(isTerminal(rightSideRule[i])){
                        if(isInVector(rightSideRule[i], column.second)) continue;
                        column.second.push_back(rightSideRule[i]);
                        notChanged = false;
                        break;
                    }

                    std::vector<char> first = getFirst_k(table, rightSideRule[i]);
                    if(first.empty()) break;
                    for (char a: first) {
                        if (a == epsilon) continueFlag = true;
                        if (!isInVector(a, column.second)) {
                            column.second.push_back(a);
                            notChanged = false;
                        }
                        if (!continueFlag) break;
                    }
                }
            }
        }

    }while (!notChanged);
    return table;
}

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

    std::vector<std::pair<char, std::vector<char>>> res = first_k(grammar);
    for (const auto& pair : res) {
        char nonTerminal = pair.first;
        std::vector<char> firstSet = pair.second;

        std::cout << "First(" << nonTerminal << ") = {";
        for (size_t i = 0; i < firstSet.size(); ++i) {
            std::cout << firstSet[i];
            if (i < firstSet.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }

    std::vector<char> tokens{readers::ReadTokens(inputFile)};
    std::copy(tokens.cbegin(), tokens.cend(), std::ostream_iterator<char>{std::cout, " "});

    return EXIT_SUCCESS;
}
