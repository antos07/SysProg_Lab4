#include <iostream>
#include "grammar.hpp"

int main() {
    grammar::Grammar grammar{
            {'A', grammar::GrammarRuleOutput{{'a'}}},
            {'A', grammar::GrammarRuleOutput{{'b'}}},
            {'A', grammar::GrammarRuleOutput{{'a', 'A', 'b'}}},
    };

    for (const auto &[ruleInput, ruleOutput] : grammar) {
        std::cout << ruleInput << " → " << ruleOutput << '\n';
    }

    return 0;
}
