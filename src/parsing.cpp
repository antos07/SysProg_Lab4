#include <vector>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include "parsing.hpp"


using namespace std;

namespace parsing {

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

//        while(!rules.empty()) {
            char topOnLexeme = lexeme.top();
            cout << leftNonTerminal;
            if (leftNonTerminal != topOnLexeme) {
                vector <char> rightRule = grammar[(managingTable[leftNonTerminal][topOnLexeme])-1].second;
            }
//        }


    }

}
