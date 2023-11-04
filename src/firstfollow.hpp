#ifndef SYSPROG_LAB4_FIRSTFOLLOW_HPP
#define SYSPROG_LAB4_FIRSTFOLLOW_HPP

#include <iostream>
#include "grammar.hpp"
#include <vector>
#include <format>
#include <fstream>
#include <iterator>
#include <map>
#include <set>

namespace firstfollow {
    char getWordBeginning(const grammar::Grammar &g);

    std::map<char, std::set<char>> firstK(const grammar::Grammar &g);

    void displayResult(const std::map<char, std::set<char>> &result, const std::string &meth);

    std::map<char, std::set<char>> getRow(const grammar::Grammar &g);

    std::map<char, std::set<char>> followK(const grammar::Grammar &g, std::map<char, std::set<char>> firstK);
}


#endif //SYSPROG_LAB4_FIRSTFOLLOW_HPP
