//
// Created by Maks Konevych on 03.11.2023.
//

#ifndef SYSPROG_LAB4_TREE_VISUALIZATION_HPP
#define SYSPROG_LAB4_TREE_VISUALIZATION_HPP

#include <graphviz/cgraph.h>
#include <graphviz/cdt.h>
#include <graphviz/gvc.h>
#include <vector>

#include "tree.hpp"


namespace ast {

    class GraphvizVisualizer {
    public:
        GraphvizVisualizer();
        ~GraphvizVisualizer();
        void ToPNG(const std::string& path, const ast::Tree&);

    private:
        GVC_t *gvc_;

        void Helper(Agraph_s *g, const ast::Tree::Node& node, Agnode_t *gvNode, int &idCount);
    };

}


#endif //SYSPROG_LAB4_TREE_VISUALIZATION_HPP
