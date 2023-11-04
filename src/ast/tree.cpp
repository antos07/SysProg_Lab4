//
// Created by Maks Konevych on 04.11.2023.
//

#include "tree.hpp"
#include <exception>

ast::Tree::Tree(const grammar::Grammar &grammar, const std::vector<int> &appliedRules)
: grammar_{grammar}, appliedRulesIdx_{appliedRules}
{
    buildTree();
}

[[nodiscard]] const ast::Tree::Node &ast::Tree::getRoot() const {
    return root_;
}

void ast::Tree::buildTree() {
    if (appliedRulesIdx_.empty())
        return;

    // init root
    root_ = Node(grammar_[appliedRulesIdx_[0]].first);

    // queue of applied rules
    auto rulesQ = std::queue<grammar::GrammarRule>();
    for (auto ruleIdx : appliedRulesIdx_) {
        rulesQ.push(grammar_[ruleIdx]);
    }

    // recursively build AST
    buildTreeHelper(rulesQ, root_);
}

void ast::Tree::buildTreeHelper(std::queue<grammar::GrammarRule> &rulesQ, ast::Tree::Node& currentNode) {
    if (rulesQ.empty())
        return;

    auto currentRule = rulesQ.front();

    if (currentRule.first != currentNode.name) // current node is terminal
        return;

    rulesQ.pop(); // relevant rule, pop

    for (auto childName : currentRule.second.getSymbols()) {
        // add and process the children
        auto childNode = Node(childName);
        buildTreeHelper(rulesQ, childNode);
        currentNode.children.push_back(childNode);
    }
}
