#include <iostream>
#include "grammar.hpp"
#include "readers.hpp"
#include <vector>
#include <format>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <map>
#include <set>

char epsilon = 'e';

bool isTerminal(char a){
    if((islower(a))) return true;
    return false;
}

bool isInVector(char symbolToFind, const std::vector<char>& charVector) {

    for (char c : charVector) {
        if (c == symbolToFind) return true;
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

/*
std::set<char> getFirst_k(std::map<char, std::set<char>> first_kTable, char nonterm) {

    for (const auto& first_k : first_kTable) {
        char c = first_k.first;
        if(c == nonterm) return first_k.second;
    }

    return std::set<char>{};
}*/

//.!.   εつ▄█▀█●
//8====o


std::map<char, std::set<char>> first_k(const grammar::Grammar& g){

    std::map<char, std::set<char>> table;
    std::vector<char> nonTerminals = getNonterminals(g);

    for (const char c : nonTerminals) {
        table[c] = std::set<char>();
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

                        if(column.second.count(rightSideRule[i]) > 0) continue;
                        column.second.insert(rightSideRule[i]);
                        notChanged = false;
                        break;
                    }

                    std::set<char> first = table[rightSideRule[i]];
                    if(first.empty()) break;
                    for (char a: first) {
                        if (a == epsilon) continueFlag = true;

                        if(column.second.count(a) == 0) {
                            column.second.insert(a);
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


void displayFirstK(const std::map<char, std::set<char>>& result) {
    for (const auto& entry : result) {
        char nonTerminal = entry.first;
        const std::set<char>& firstKSet = entry.second;

        std::cout << "First_k(" << nonTerminal << ") = {";
        bool first = true;
        for (char terminal : firstKSet) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << terminal;
            first = false;
        }
        std::cout << "}" << std::endl;
    }
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
        std::cout << ruleInput << " - " << ruleOutput << '\n';
    }

    std::map<char, std::set<char>> result = first_k(grammar);
    displayFirstK(result);

    std::vector<char> tokens{readers::ReadTokens(inputFile)};
    std::copy(tokens.cbegin(), tokens.cend(), std::ostream_iterator<char>{std::cout, " "});

    return EXIT_SUCCESS;
}
