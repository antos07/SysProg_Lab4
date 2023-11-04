//
// Created by antos07 on 11/3/23.
//
#include <fstream>
#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <algorithm>
#include "firstfollow.hpp"
#include "parsing.hpp"

using namespace std;

namespace parsing {
    std::vector<std::set<char>> BuildIntermediateTable(const grammar::Grammar &grammar) {
        auto firstK = firstfollow::firstK(grammar);
        auto followK = firstfollow::followK(grammar, firstK);
        std::vector<std::set<char>> table{};
        for (const auto &[inputNonTerminal, output]: grammar) {
            if (output.getSymbols()[0] == grammar::epsilon) {
                // FirstK of epsilon is epsilon, so adding only followK(inputNonTerminal)
                table.push_back(followK[inputNonTerminal]);
                continue;
            }
            if (grammar::isTerminal(output.getSymbols()[0])) {
                // rule output starts with a terminal. So it's the only possible symbol.
                table.push_back({output.getSymbols()[0]});
                continue;
            }

            table.emplace_back();
            for (char symbol : output.getSymbols()) {
                if (grammar::isTerminal(symbol)) {
                    table.back().insert(symbol);

                    // Remove epsilon from the set — impossible.
                    table.back().erase(grammar::epsilon);

                    break;
                }

                // Add first_k of the current non-terminal.
                table.back().insert(firstK[symbol].cbegin(), firstK[symbol].cend());

                if (!firstK[symbol].contains(grammar::epsilon)) {
                    // Impossible to get further. Calculated first_k of the rule output.

                    // Remove epsilon from the set — impossible.
                    table.back().erase(grammar::epsilon);
                    break;
                }
            }

            if (table.back().contains(grammar::epsilon)) {
                // Add followK of the inputNonTerminal, because the firstK of the
                // output can be empty.
                table.back().insert(followK[inputNonTerminal].cbegin(), followK[inputNonTerminal].cend());

                if (!followK[inputNonTerminal].contains(grammar::epsilon)) {
                    // Epsilon is impossible in the concatenation.
                    table.back().erase(grammar::epsilon);
                }
            }
        }

        return table;
    }
     void createManagingTable(vector<set<char>> terminalsInRules, grammar::Grammar grammar, map<char, map<char, int>> &managingTable) {
        bool checkedRules[8];
        fill(checkedRules, checkedRules+8, false);
        for (int i = 1; i <= 8; i++) {
            if (!checkedRules[i-1]) {
                map<char, int> symbolMap;
                char leftNonTerminal = grammar[i-1].first;
                for (int k = 0; k < terminalsInRules[i-1].size(); k++) {
                    for (char sym : terminalsInRules[i-1]) {
                        symbolMap.insert({sym, i});
                    }
                }
                for (int j = i+1; j <= 8; j++) {
                    if (grammar[j-1].first == leftNonTerminal and !checkedRules[j-1]) {
                        for (int k = 0; k < terminalsInRules[i-1].size(); k++) {
                            for (char sym : terminalsInRules[j-1]) {
                                symbolMap.insert({sym, j});
                            }
                        }
                        checkedRules[j-1] = true;
                    }
                }
                managingTable.insert({leftNonTerminal, symbolMap});
                checkedRules[i-1] = true;
            }
        }

        //Test output of managing table
        for (const auto& outerPair : managingTable) {
            char outerKey = outerPair.first;
            const std::map<char, int>& innerMap = outerPair.second;

            for (const auto& innerPair : innerMap) {
                char innerKey = innerPair.first;
                int innerValue = innerPair.second;

                std::cout << "Outer Key: " << outerKey
                          << ", Inner Key: " << innerKey
                          << ", Value: " << innerValue << std::endl;
            }
        }
        //
    }


    void syntacticAnalysis(grammar::Grammar grammar) {
        map<char, map<char, int>> managingTable;
        vector <char> lex = {'(','a','+','a',')','*','a'};
        vector<char> terminals = {'a', '(', ')', '+', '*', 'e'};
        vector<char> nonTerminals = {'S', 'A', 'B', 'C', 'D'};
        vector<set<char>> terminalsInRules = {{'(', 'a'}, {'+'}, {'e', ')'}, {'(', 'a'},
                                              {'*'}, {'+', 'e', ')'}, {'('}, {'a'} };
        createManagingTable(terminalsInRules, grammar, managingTable);
        stack<char> rules;
        stack<char> lexeme;

        for(int i = lex.size()-1; i >= 0; i--) {
            lexeme.push(lex[i]);
        }
        char leftNonTerminal = grammar[0].first;
        rules.push(leftNonTerminal);
        bool error_exist = false;

        do {
            char topOnLexeme = lexeme.top();
            if (leftNonTerminal != topOnLexeme) {
                cout <<managingTable[leftNonTerminal][topOnLexeme] << " ";
                vector <char> rightRule = grammar[(managingTable[leftNonTerminal][topOnLexeme])-1].second;
                for (int j = rightRule.size()-1; j >= 0; j--) {
                    rules.push(rightRule[j]);
                }
            }
            else {
                lexeme.pop();
            }
            rules.pop();
        }
        while(!rules.empty());


    }
}