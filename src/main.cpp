#include <iostream>
#include <set>
#include <map>
#include "grammar.hpp"
#include "parsing.hpp"

int main() {
    grammar::Grammar grammar{
            {'S', grammar::GrammarRuleOutput{{'B', 'A'}}},
            {'A', grammar::GrammarRuleOutput{{'+', 'B', 'A'}}},
            {'A', grammar::GrammarRuleOutput{{'e'}}},
            {'B', grammar::GrammarRuleOutput{{'D', 'C'}}},
            {'C', grammar::GrammarRuleOutput{{'*', 'D', 'C'}}},
            {'C', grammar::GrammarRuleOutput{{'e'}}},
            {'D', grammar::GrammarRuleOutput{{'(', 'S', ')'}}},
            {'D', grammar::GrammarRuleOutput{{'a'}}},
    };

    for (const auto &[ruleInput, ruleOutput] : grammar) {
        std::cout << ruleInput << " → " << ruleOutput << '\n';
    }



    // Test for parsing
    vector<char> terminals = {'a', '(', ')', '+', '*', 'e'};
    vector<char> nonTerminals = {'S', 'A', 'B', 'C', 'D'};
    vector<set<char>> terminalsInRules = {{'(', 'a'}, {'+'}, {'e', ')'}, {'(', 'a'},
                                          {'*'}, {'+', 'e', ')'}, {'('}, {'a'} };
    parsing::syntacticAnalysis(grammar);
    //

    return 0;
}
