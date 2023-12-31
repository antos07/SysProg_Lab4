//
// Created by antos07 on 10/25/23.
//

#include <sstream>
#include <iterator>
#include <regex>
#include <utility>
#include "readers.hpp"

namespace readers {
    static grammar::GrammarRule ParseRule(std::string &&line) {
        std::regex ruleRegex{R"(\s*([A-Z])\s*→\s*((?:\S\s*)+))"};

        std::smatch match;
        if (!std::regex_match(line, match, ruleRegex)) {
            throw ReaderException("Incorrect rule format.");
        }

        char ruleInput{match[1].str()[0]}; // It's the first symbol of the first match.

        std::istringstream parser{match[2].str()}; // Init with the right side of the rule.
        std::vector<char> ruleOutputSequence{};
        char ruleOutputElement{};
        while (parser >> ruleOutputElement) {
            ruleOutputSequence.emplace_back(ruleOutputElement);
        }

        return {ruleInput, grammar::GrammarRuleOutput{std::move(ruleOutputSequence)}};
    }

    /**
     * Read grammar from istream.
     *
     * The grammar is represented by a set of rules (each on a new line). Rules
     * has the following format: A → B c, where capital letters represent non-terminals and
     * lowercase letters represent terminals.
     *
     * The rows are read until EOF.
     */
    grammar::Grammar ReadGrammar(std::istream &in) {
        grammar::Grammar inputGrammar{};
        while (!in.eof()) {
            std::string line{};
            std::getline(in, line);
            if (std::regex_match(line, std::regex {"^\\s*$"})) {
                // An empty string.
                continue;
            }

            inputGrammar.emplace_back(ParseRule(std::move(line)));
        }

        return inputGrammar;
    }

    std::vector<char> ReadTokens(std::istream &in) {
        return {std::istream_iterator<char>{in}, std::istream_iterator<char>{}};
    }

    ReaderException::ReaderException(std::string message) : message_{std::move(message)} {
    }

    const char *ReaderException::what() const noexcept {
        return message_.c_str();
    }
}