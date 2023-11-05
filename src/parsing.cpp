#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <vector>
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
        bool checkedRules[grammar.size()];
        fill(checkedRules, checkedRules+grammar.size(), false);
        for (int i = 1; i <= grammar.size(); i++) {
            if (!checkedRules[i-1]) {
                map<char, int> symbolMap;
                char leftNonTerminal = grammar[i-1].first;
                for (int k = 0; k < terminalsInRules[i-1].size(); k++) {
                    for (char sym : terminalsInRules[i-1]) {
                        symbolMap.insert({sym, i});
                    }
                }
                for (int j = i+1; j <= grammar.size(); j++) {
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
    }

    std::pair<bool, std::vector<int>> syntacticAnalysis(grammar::Grammar grammar, vector<char> symbolsInLexeme) {
        map<char, map<char, int>> managingTable;
        vector<set<char>> terminalsInRules = BuildIntermediateTable(grammar);
        vector<int> chainAnalysis;
        createManagingTable(terminalsInRules, grammar, managingTable);
        stack<char> rules;
        stack<char> lexeme;
        lexeme.push('e');

        for(int i = symbolsInLexeme.size()-1; i >= 0; i--) {
            lexeme.push(symbolsInLexeme[i]);
        }
        bool error_exist = false;
        char leftNonTerminal = grammar[0].first;
        rules.push(leftNonTerminal);

        cout << endl << "Lexeme: ";
        do {
            leftNonTerminal = rules.top();
            rules.pop();
            char topOnLexeme = lexeme.top();
            if (leftNonTerminal != topOnLexeme) {
                if (managingTable[leftNonTerminal].find(topOnLexeme) == managingTable[leftNonTerminal].end()) {
                    error_exist = true;
                    cout << lexeme.top();
                    break;
                }
                chainAnalysis.push_back(managingTable[leftNonTerminal][topOnLexeme]);

                vector <char> rightRule = grammar[(managingTable[leftNonTerminal][topOnLexeme])-1].second.getSymbols();
                if (rightRule[0] != 'e') {
                    for (int j = rightRule.size() - 1; j >= 0; j--) {
                        rules.push(rightRule[j]);
                    }
                }
            }
            else {
                if (lexeme.top() != 'e') {
                    cout << lexeme.top() << " ";
                    lexeme.pop();
                }
            }
        }
        while(!rules.empty());

        //Result output
        if (error_exist) {
            cout << endl;
            cout << "Syntax error on symbol " << lexeme.top();
            return {false, {}};
        }
        cout << endl;
        cout << "Chain of used rules: ";
        for (int rule_num : chainAnalysis) {
            cout << rule_num << " ";
        }
        cout << '\n';

        // Get indices of applied rules
        std::vector<int> appliedRulesIdx = chainAnalysis;
        for (int & i : appliedRulesIdx) {
            --i;
        }

        return {true, appliedRulesIdx};
    }
}