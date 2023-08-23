#include "../include/concise.hpp"

Concise::Concise(const std::string &graph) : Graph(graph)
{

    f_max_in.resize(nodes, 0);
    f_max_out.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    v_max_out.resize(nodes, -1);
    trie.resize(nodes);
    partial_result.resize(nodes);

    for (int i = 0; i < nodes; i++)
    {
        for (auto edge : adjacency_list[i])
        {

            int u = i;
            int v = edge.first;
            double w = edge.second;

            if (f_max_in[v] < w)
            {
                f_max_in[v] = w;
                v_max_in[v] = u;
            }
            if (f_max_out[u] < w)
            {
                f_max_out[u] = w;
                v_max_out[u] = v;
            }
        }
    }

    std::vector<bool> visited(nodes, false);
    for (int i = 0; i < nodes; i++)
    {
        if (!visited[i])
        {
            topo_dfs(i, visited);
        }
    }
    std::reverse(topo_order.begin(), topo_order.end());

    for (int i = 0; i < nodes; i++)
        trie[i] = std::make_unique<Trie>(i);
}

void Concise::compute_safe(int u)
{

    // int v_star;
    // if((u!=source)&&(u!=sink)) v_star = v_max_out[u];
    // else v_star = -1;

    // Node* current_node;

    // for(std::pair<int,double> p: adjacency_list[u]) {
    //     int v = p.first;
    //     if(v==v_star) continue;
    //     int f_x = p.second;

    //     current_node = trie[v]->insert(u,f_x,trie[v]->head);
    //     int x = u;

    //     while((x!=source)&&(f_x-f_in[x]+f_max_in[x]>0)) {
    //         int k = v_max_in[x];
    //         current_node = trie[v]->insert(k,f_max_in[x],current_node);
    //         f_x += f_max_in[x] - f_in[x];
    //         x = k;
    //     }
    //     leaves[v].push_back({current_node,f_x});
    // }

    // if(v_star!=-1) {
    //     trie[v_star]->insert(trie[u],f_max_out[u],trie[v_star]->head);
    // }

    // for(std::pair<Node*,double> p: leaves[u]) {
    //     Node* x = p.first;
    //     int f_x = p.second;
    //     double excess = f_x-f_out[u]+f_max_out[u];

    //     if((v_star==-1)||(excess<=0)) {

    //         std::list<int> path;
    //         bool invalid = false;
    //         current_node = x;
    //         while(current_node->parent_node!=trie[u]->head) {
    //             path.push_back(current_node->value);
    //             current_node=current_node->parent_node;
    //             if(current_node==nullptr) {
    //                 invalid = true;
    //                 break;
    //             }
    //         }
    //         if(invalid) continue;
    //         path.push_back(current_node->value);
    //         path.push_back(u);
    //         path.reverse();
    //         concise_decomposition.push_back({f_x,path});
    //     }

    //     if(v_star!=-1) {
    //         f_x = excess;
    //         while((f_x<=0)&&(x->children==0)) {
    //             Node* y = x->parent_node;
    //             f_x = f_x+f_in[y->value]-x->flow_to_parent;
    //             x->parent_node=nullptr;
    //             y->children--;
    //             x = y;
    //         }
    //         if(x->children==0) {
    //             leaves[v_star].push_back({x,f_x});
    //         }
    //     }
    // }
    return;
}

void Concise::print_maximal_safe_paths()
{
    //
    return;
}

void Concise::topo_dfs(int v, std::vector<bool> &visited)
{
    visited[v] = true;
    for (std::pair<int, double> u : adjacency_list[v])
    {
        if (!visited[u.first])
        {
            topo_dfs(u.first, visited);
        }
    }
    topo_order.push_back(v);
    return;
}
