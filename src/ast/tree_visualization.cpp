//
// Created by Maks Konevych on 04.11.2023.
//

#include "tree_visualization.hpp"


ast::GraphvizVisualizer::GraphvizVisualizer() {
    gvc_ = gvContext();
}

ast::GraphvizVisualizer::~GraphvizVisualizer() {
    gvFreeContext(gvc_);
}

void ast::GraphvizVisualizer::ToPNG(const std::string &path, const ast::Tree &tree) {
    // init graph and root
    Agraph_t *g = agopen("g", Agstrictdirected, 0);
    const auto &root = tree.getRoot();
    int idCount = 1;    // keeps track of unique node identifiers
    char rootName[2] {root.name};
    Agnode_t *gvRoot = agnode(g, rootName, idCount);
    agsafeset(gvRoot, "color", "green", "");
    agsafeset(gvRoot, "style", "bold", "");

    // recursively build the graph
    Helper(g, root, gvRoot, idCount);

    // set layout
    gvLayout(gvc_, g, "dot");

    //
    gvRenderFilename(gvc_, g, "png", path.c_str());

    //
    gvFreeLayout(gvc_, g);
    agclose(g);
}

void ast::GraphvizVisualizer::Helper(Agraph_s *g, const ast::Tree::Node &node, Agnode_t *agNode, int &idCount) {
    for (const auto &child : node.children) {
        // add the child to the graph
        auto unqName = std::to_string(++idCount);   // unique name of the vertex
        Agnode_t *agChild = agnode(g, const_cast<char *>(unqName.c_str()), idCount);
        char label[2] {child.name}; // label of the vertex
        agsafeset(agChild, "label", label, "");

        // connect to the parent
        Agedge_s *agEdge = agedge(g, agNode, agChild, 0, 1);

        // recursively process the child
        Helper(g, child, agChild, idCount);
    }
}
