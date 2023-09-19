#include "../include/raw.hpp"

Raw::Raw(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    trie.resize(nodes);
    leaves.resize(nodes);

    for (int i = 0; i < nodes; i++)
    {
        for (auto &&edge : adjacency_list[i])
        {

            int u = i;
            int v = edge.first;
            double w = edge.second;

            if (f_max_in[v] < w)
            {
                f_max_in[v] = w;
                v_max_in[v] = u;
            }
            f_in[v] += w;
        }
    }

    std::vector<bool> visited(nodes, false);
    for (int i = 0; i < nodes; i++)
    {
        if (!visited[i])
            topo_dfs(i, visited);
    }
    std::reverse(topo_order.begin(), topo_order.end());

    for (int i = 0; i < nodes; i++)
        trie[i] = std::make_unique<Trie>(i);
}

void Raw::compute_safe(int u)
{
    int v_max_out_u = -1;
    double f_max_out_u = 0;
    double f_out_u = 0;
    for (auto &&p : adjacency_list[u])
    {
        f_out_u += p.second;
        if (f_max_out_u < p.second)
        {
            f_max_out_u = p.second;
            v_max_out_u = p.first;
        }
    }

    int v_star;
    if ((f_in[u] != 0) && (f_out_u != 0))
        v_star = v_max_out_u;
    else
        v_star = -1;

    std::shared_ptr<Node> current_node;

    for (auto &&p : adjacency_list[u])
    {
        int v = p.first;
        if (v == v_star)
            continue;
        double f_x = p.second;

        current_node = trie[v]->insert(u, f_x, trie[v]->head);
        int x = u;

        while ((f_in[x] != 0) && (f_x - f_in[x] + f_max_in[x] > 0))
        {
            int k = v_max_in[x];
            current_node = trie[v]->insert(k, f_max_in[x], current_node);
            f_x += f_max_in[x] - f_in[x];
            x = k;
        }
        leaves[v].push_back({std::move(current_node), f_x});
    }

    if (v_star != -1)
        trie[v_star]->insert(trie[u], f_max_out_u, trie[v_star]->head);

    for (auto &&p : leaves[u])
    {
        std::shared_ptr<Node> x = p.first;
        double f_x = p.second;
        double excess = f_x - f_out_u + f_max_out_u;

        if ((v_star == -1) || (excess <= 0))
        {

            std::list<int> path;
            bool invalid = false;
            current_node = x;
            while (current_node->parent_node != trie[u]->head)
            {
                path.push_back(current_node->value);
                current_node = current_node->parent_node;
                if (current_node == nullptr)
                {
                    invalid = true;
                    break;
                }
            }
            if ((!invalid) && (!path.empty()))
            {
                path.push_back(current_node->value);
                path.push_back(u);
                raw_repr.push_back({f_x, std::vector<int>{std::make_move_iterator(std::begin(path)),
                                                          std::make_move_iterator(std::end(path))}});
            }
        }

        if (v_star != -1)
        {
            f_x = excess;
            while ((f_x <= 0) && (x->children == 0))
            {
                std::shared_ptr<Node> y = x->parent_node;
                f_x = f_x + f_in[y->value] - x->flow_to_parent;
                x->parent_node.reset();
                y->children--;
                x = y;
            }
            if (x->children == 0)
                leaves[v_star].push_back({x, f_x});
        }
    }
    return;
}

void Raw::print_maximal_safe_paths()
{
    std::cout << metadata << "\n";
    for (auto &&path : raw_repr)
    {
        std::cout << path.first << " ";
        for (auto &&value : path.second)
            std::cout << value << " ";
        std::cout << "\n";
    }
    return;
}

void Raw::calculate_statistics()
{
    total_nodes += nodes;
    total_edges += edges;
    for (auto &&path : raw_repr)
        length += path.second.size() + 1;
    return;
}

void Raw::topo_dfs(int v, std::vector<bool> &visited)
{
    visited[v] = true;
    for (auto &&u : adjacency_list[v])
    {
        if (!visited[u.first])
            topo_dfs(u.first, visited);
    }
    topo_order.push_back(v);
    return;
}
