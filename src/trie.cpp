#include "../include/trie.hpp"

Node::Node(int u) {
    value = u;
    children = 0;
    flow_to_parent = 0.0;
    parent_node = nullptr;
}

Node::Node(int u, double flow, Node* parent) {
    value = u;
    children = 0;
    flow_to_parent = flow;
    parent_node = parent;
}

Trie::Trie(int u) {
    head = new Node(u);
}

Node* Trie::insert(int u, double flow, Node* parent) {
    parent->children++;
    Node* node = new Node(u,flow,parent);
    return node;
}

void Trie::insert(Trie* u, double flow, Node* parent) {
    parent->children++;
    u->head->parent_node = parent;
    u->head->flow_to_parent = flow;
    return;
}