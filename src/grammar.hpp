//
// Created by antos07 on 10/25/23.
//

#ifndef SYSPROG_LAB4_GRAMMAR_HPP
#define SYSPROG_LAB4_GRAMMAR_HPP

#include <string>
#include <vector>

namespace grammar {
    class GrammarRuleOutput {
        std::vector<char> symbols_;

    public:
        GrammarRuleOutput() = delete;

        explicit GrammarRuleOutput(std::vector<char> &&symbols);
    };

    using GrammarRule = std::pair<char, GrammarRuleOutput>;
    using Grammar = std::vector<GrammarRule>;
}

#endif //SYSPROG_LAB4_GRAMMAR_HPP
