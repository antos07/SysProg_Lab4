//
// Created by antos07 on 10/25/23.
//

#include <algorithm>
#include <iterator>
#include <sstream>
#include <utility>
#include "grammar.hpp"

namespace grammar {
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
}
