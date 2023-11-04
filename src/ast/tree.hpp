//
// Created by Maks Konevych on 04.11.2023.
//

#ifndef SYSPROG_LAB4_TREE_HPP
#define SYSPROG_LAB4_TREE_HPP

#include "../grammar.hpp"
#include <utility>
#include <vector>
#include <queue>

namespace ast {
    class Tree {
    public:
        struct Node {
            char name;
            std::vector<Node> children;

            explicit Node(char name_) :name{name_}, children{std::vector<Node>()} {}
            Node(char name_, std::vector<Node> children_) : name{name_}, children{std::move(children_)} {}
        };

        Tree(const grammar::Grammar& grammar, const std::vector<int>& appliedRules);

        [[nodiscard]] const Node &getRoot() const;
    private:
        Node root_ = Node{'e'};
        const grammar::Grammar &grammar_;
        const std::vector<int> &appliedRulesIdx_;

        void buildTree();
        void buildTreeHelper(std::queue<grammar::GrammarRule>&, Node&);
    };
}


#endif //SYSPROG_LAB4_TREE_HPP
