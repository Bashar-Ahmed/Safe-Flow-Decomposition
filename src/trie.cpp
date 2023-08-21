#include "../include/trie.hpp"

Node::Node(int u) {
    value = u;
    children = 0;
    flow_to_parent = 0.0;
    parent_node = nullptr;
}

Node::Node(int u, double flow, std::shared_ptr<Node> parent) {
    value = u;
    children = 0;
    flow_to_parent = flow;
    parent_node = parent;
}

Trie::Trie(int u) {
    head = std::make_shared<Node>(u);
}

std::shared_ptr<Node> Trie::insert(int u, double flow, std::shared_ptr<Node> parent) {
    parent->children++;
    std::shared_ptr<Node> node = std::make_shared<Node>(u,flow,parent);
    return node;
}

void Trie::insert(std::unique_ptr<Trie>& u, double flow, std::shared_ptr<Node> parent) {
    parent->children++;
    u->head->parent_node = parent;
    u->head->flow_to_parent = flow;
    return;
}