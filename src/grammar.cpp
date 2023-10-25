//
// Created by antos07 on 10/25/23.
//

#include <utility>
#include "grammar.hpp"

namespace grammar {
    GrammarRuleOutput::GrammarRuleOutput(std::vector<char> &&symbols) : symbols_{std::move(symbols)} {
    }
}