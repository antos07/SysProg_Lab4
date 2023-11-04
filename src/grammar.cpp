//
// Created by antos07 on 10/25/23.
//

#include <algorithm>
#include <iterator>
#include <sstream>
#include <utility>
#include "grammar.hpp"

namespace grammar {
    const char epsilon = 'e';

    GrammarRuleOutput::GrammarRuleOutput(std::vector<char> &&symbols) : symbols_{std::move(symbols)} {
    }

    std::ostream &operator<<(std::ostream &out, const GrammarRuleOutput &grammarRuleOutput) {
        std::copy(grammarRuleOutput.symbols_.cbegin(), grammarRuleOutput.symbols_.cend(),
                  std::ostream_iterator<char>{out, " "});
        return out;
    }

    std::string GrammarRuleOutput::toString() const {
        std::stringstream converter{};
        converter << *this;
        return converter.str();
    }

    bool isTerminal(char a) {
        if (!std::isupper(a) && ! std::isspace(a)) return true;
        return false;
    }

    bool isInVector(char symbolToFind, const std::vector<char> &charVector) {

        for (char c: charVector) {
            if (c == symbolToFind) return true;
        }

        return false;
    }

    std::vector<char> getNonTerminals(const Grammar &g) {

        std::vector<char> nonTerminals;
        for (const auto &rule: g) {
            char c = rule.first;
            if (!isInVector(c, nonTerminals)) {
                nonTerminals.push_back(c);
            }
        }
        return nonTerminals;
    }
}
