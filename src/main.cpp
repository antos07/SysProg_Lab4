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
#include "regex"
#include "firstfollow.h"

char epsilon = 'e';

bool isTerminal(char a) {
    if (islower(a)) return true;
    return false;
}

bool isInVector(char symbolToFind, const std::vector<char> &charVector) {

    for (char c: charVector) {
        if (c == symbolToFind) return true;
    }
    return false;
}

std::vector<char> getNonterminals(const grammar::Grammar &g) {

    std::vector<char> nonTerminals;
    for (const auto &rule: g) {
        char c = rule.first;
        if (!isInVector(c, nonTerminals)) {
            nonTerminals.push_back(c);
        }
    }
    return nonTerminals;
}

//get word beginning nonterminal
char getWordBeginning(const grammar::Grammar &g) {
    for (const auto &rule: g)
        if (!isTerminal(rule.first)) return rule.first;
}


//.!.   εつ▄█▀█● <--- Anton
//8====o


std::map<char, std::set<char>> firstK(const grammar::Grammar &g) {

    std::map<char, std::set<char>> table;
    std::vector<char> nonTerminals = getNonterminals(g);

    for (const char c: nonTerminals) {
        table[c] = std::set<char>();
    }

    bool notChanged;

    do {

        notChanged = true;
        for (auto &column: table) {

            char nonTerminal = column.first;
            for (const auto &rule: g) {

                char leftSideRule = rule.first;
                if (leftSideRule != nonTerminal) continue;
                auto rightSideRule = rule.second.getSymbols();

                bool continueFlag = false;
                for (char i: rightSideRule) {

                    if (isTerminal(i)) {

                        if (column.second.count(i) == 0) {
                            notChanged = false;
                            column.second.insert(i);
                        }
                        break;
                    }

                    std::set<char> first = table[i];
                    if (first.empty()) break;

                    for (char a: first) {

                        if (a == epsilon) continueFlag = true;
                        if (column.second.count(a) == 0) {
                            notChanged = false;
                            column.second.insert(a);
                        }
                    }

                    if (!continueFlag) break;
                }
            }
        }

    } while (!notChanged);
    return table;
}

void displayFirstK(const std::map<char, std::set<char>> &result) {

    for (const auto &entry: result) {
        char nonTerminal = entry.first;
        const std::set<char> &firstKSet = entry.second;

        std::cout << "First_k(" << nonTerminal << ") = {";
        bool first = true;
        for (char terminal: firstKSet) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << terminal;
            first = false;
        }
        std::cout << "}" << std::endl;
    }
}

std::map<char, std::set<char>> getRow(const grammar::Grammar &g) {
    std::map<char, std::set<char>> row;
    std::vector<char> nonTerminals = getNonterminals(g);
    for (const char c: nonTerminals) {
        row[c] = std::set<char>();
    }

    return row;
}

std::map<char, std::set<char>> followK(const grammar::Grammar &g, std::map<char, std::set<char>> firstK) {
    std::map<char, std::set<char>> row = getRow(g);

    row[getWordBeginning(g)].insert(epsilon);

    std::vector<std::map<char, std::set<char>>> table;
    table.push_back(row);

    bool continueFlag = true;

    while (continueFlag) {
        continueFlag = false;
        row = getRow(g);
        std::map<char, std::set<char>> prevRow = table.back();

        for (auto &follow: row) {

            for (const auto &rule: g) {
                if (!prevRow[rule.first].empty()) {
                    std::vector<char> leftPart = rule.second.getSymbols();

                    if (isInVector(follow.first, leftPart)) {
                        std::vector<std::vector<char>> subParts;

                        for (int i = 0; i < leftPart.size(); i++) {
                            if (leftPart[i] == follow.first) {
                                std::vector<char> subPart = {leftPart.begin() + i + 1, leftPart.end()};

                                subParts.emplace_back(subPart);
                            }
                        }

                        for (auto subPart: subParts) {

                            std::set<char> epsilonCheck;
                            subPart.push_back(epsilon);

                            for (char symbol: subPart) {
                                if (isTerminal(symbol)) {
                                    epsilonCheck.insert(symbol);
                                    break;
                                } else {
                                    epsilonCheck.insert(firstK[symbol].begin(), firstK[symbol].end());

                                    if (epsilonCheck.count(epsilon) == 0) break;
                                    else epsilonCheck.erase(epsilon);
                                }
                            }

                            if (epsilonCheck.count(epsilon) != 0) {
                                epsilonCheck.erase(epsilon);
                                epsilonCheck.insert(prevRow[rule.first].begin(), prevRow[rule.first].end());
                            }

                            follow.second.insert(epsilonCheck.begin(), epsilonCheck.end());
                            follow.second.insert(prevRow[follow.first].begin(), prevRow[follow.first].end());
                        }
                    }
                }

            }
        }

        table.push_back(row);

        if (row != prevRow) continueFlag = true;
    }
    return table.back();
}

int main(int argc, char *argv[]) {

    //Anton, ich habe deine Mutter geficked
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
    for (const auto &[ruleInput, ruleOutput]: grammar) {
        std::cout << ruleInput << " - " << ruleOutput << '\n';
    }

    std::map<char, std::set<char>> result = firstK(grammar);
    displayFirstK(result);

    std::map<char, std::set<char>> result2 = followK(grammar, firstK(grammar));
    displayFirstK(result2);

    std::vector<char> tokens{readers::ReadTokens(inputFile)};
    std::copy(tokens.cbegin(), tokens.cend(), std::ostream_iterator<char>{std::cout, " "});

    return EXIT_SUCCESS;
}
