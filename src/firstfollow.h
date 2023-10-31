#ifndef SYSPROG_LAB4_FIRSTFOLLOW_H
#define SYSPROG_LAB4_FIRSTFOLLOW_H

#include <iostream>
#include "grammar.hpp"
#include <vector>
#include <format>
#include <fstream>
#include <iterator>
#include <map>
#include <set>

bool isTerminal(char a);

bool isInVector(char symbolToFind, const std::vector<char> &charVector);

std::vector<char> getNonTerminals(const grammar::Grammar &g);

char getWordBeginning(const grammar::Grammar &g);

std::map<char, std::set<char>> firstK(const grammar::Grammar &g);

void displayResult(const std::map<char, std::set<char>> &result, const std::string &meth);

std::map<char, std::set<char>> getRow(const grammar::Grammar &g);

std::map<char, std::set<char>> followK(const grammar::Grammar &g, std::map<char, std::set<char>> firstK);


#endif //SYSPROG_LAB4_FIRSTFOLLOW_H
