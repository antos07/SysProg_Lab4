//
// Created by antos07 on 11/3/23.
//

#ifndef SYSPROG_LAB4_PARSING_HPP
#define SYSPROG_LAB4_PARSING_HPP

#include <set>
#include <vector>
#include "grammar.hpp"

namespace parsing {
    std::vector<std::set<char>> BuildIntermediateTable(const grammar::Grammar &grammar);
}

#endif //SYSPROG_LAB4_PARSING_HPP
