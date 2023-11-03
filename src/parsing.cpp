//
// Created by antos07 on 11/3/23.
//

#include <cassert>
#include <algorithm>
#include "firstfollow.hpp"
#include "parsing.hpp"

namespace parsing {
    std::vector<std::set<char>> BuildIntermediateTable(const grammar::Grammar &grammar) {
        auto firstK = firstfollow::firstK(grammar);
        auto followK = firstfollow::followK(grammar, firstK);
        std::vector<std::set<char>> table{};
        for (const auto &[inputNonTerminal, output]: grammar) {
            if (output.getSymbols()[0] == grammar::epsilon) {
                // FirstK of epsilon is epsilon, so adding only followK(inputNonTerminal)
                table.push_back(followK[inputNonTerminal]);
                continue;
            }
            if (grammar::isTerminal(output.getSymbols()[0])) {
                // rule output starts with a terminal. So it's the only possible symbol.
                table.push_back({output.getSymbols()[0]});
                continue;
            }

            table.emplace_back();
            for (char symbol : output.getSymbols()) {
                if (grammar::isTerminal(symbol)) {
                    table.back().insert(symbol);

                    // Remove epsilon from the set — impossible.
                    table.back().erase(grammar::epsilon);

                    break;
                }

                // Add first_k of the current non-terminal.
                table.back().insert(firstK[symbol].cbegin(), firstK[symbol].cend());

                if (!firstK[symbol].contains(grammar::epsilon)) {
                    // Impossible to get further. Calculated first_k of the rule output.

                    // Remove epsilon from the set — impossible.
                    table.back().erase(grammar::epsilon);
                    break;
                }
            }

            if (table.back().contains(grammar::epsilon)) {
                // Add followK of the inputNonTerminal, because the firstK of the
                // output can be empty.
                table.back().insert(followK[inputNonTerminal].cbegin(), followK[inputNonTerminal].cend());

                if (!followK[inputNonTerminal].contains(grammar::epsilon)) {
                    // Epsilon is impossible in the concatenation.
                    table.back().erase(grammar::epsilon);
                }
            }
        }

        return table;
    }
}
