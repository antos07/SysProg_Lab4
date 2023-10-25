#include <iostream>
#include "grammar.hpp"
#include "readers.hpp"

int main() {
    grammar::Grammar grammar{readers::ReadGrammar(std::cin)};

    for (const auto &[ruleInput, ruleOutput] : grammar) {
        std::cout << ruleInput << " → " << ruleOutput << '\n';
    }

    return 0;
}
