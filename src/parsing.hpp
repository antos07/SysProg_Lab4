#ifndef SYSPROG_LAB4_PARSING_HPP
#define SYSPROG_LAB4_PARSING_HPP

#include <vector>
#include <set>
#include "grammar.hpp"

using namespace std;

namespace parsing {
    void createManagingTable(vector<set<char>> terminalsInRules, grammar::Grammar grammar, map<char, map<char, int>> &managingTable);
    void syntacticAnalysis(grammar::Grammar grammar);
}


#endif //SYSPROG_LAB4_PARSING_HPP
