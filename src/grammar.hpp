//
// Created by antos07 on 10/25/23.
//

#ifndef SYSPROG_LAB4_GRAMMAR_HPP
#define SYSPROG_LAB4_GRAMMAR_HPP

#include <iostream>
#include <string>
#include <vector>

namespace grammar {
    class GrammarRuleOutput {
        std::vector<char> symbols_;

    public:
        GrammarRuleOutput() = delete;

        explicit GrammarRuleOutput(std::vector<char> &&symbols);

        const std::vector<char> &getSymbols() const {
            return symbols_;
        }

        friend std::ostream &operator<<(std::ostream &out, const GrammarRuleOutput &grammarRuleOutput);

        [[nodiscard]] std::string toString() const;
    };


    using GrammarRule = std::pair<char, GrammarRuleOutput>;
    using Grammar = std::vector<GrammarRule>;

    bool isTerminal(char a);

    bool isInVector(char symbolToFind, const std::vector<char> &charVector);

    std::vector<char> getNonTerminals(const Grammar &g);
}

#endif //SYSPROG_LAB4_GRAMMAR_HPP
