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

                if (isTerminal(rightSideRule[0])){

                    if(isInVector(rightSideRule[0], column.second)) continue;
                    column.second.push_back(rightSideRule[0]);
                    notChanged = false;
                    
                }else{

                    std::vector<char> first = getFirst_k(table, rightSideRule[0]);
                    if(first.empty()) continue;
                    for (char a : first) {
                        if(isInVector(a, column.second)) continue;
                        column.second.push_back(a);
                        notChanged = false;
                    }

                }
            }
        }

    }while (!notChanged);

    return table;
}

int main(){

    grammar::Grammar grammar{readers::ReadGrammar(std::cin)};
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

    return 0;
}
