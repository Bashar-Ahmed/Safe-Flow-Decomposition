#pragma once

struct Node {
    int value, children;
    double flow_to_parent;
    Node* parent_node;
    Node(int u);
    Node(int u, double flow, Node* parent);
};

struct Trie {
    Node* head;
    Trie(int u);
    Node* insert(int u, double flow, Node* parent);
    void insert(Trie* u, double flow, Node* parent);
};
