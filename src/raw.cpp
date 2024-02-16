#include "raw.hpp"

Raw::Raw(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    leaves.resize(nodes);

    trie.reserve(nodes);

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
        if (f_in[i] == 0)
            topo_dfs(i, visited);
    }
    std::reverse(topo_order.begin(), topo_order.end());

    for (int i = 0; i < nodes; i++)
        trie.emplace_back(std::make_unique<Path_Trie<Raw_Node>>(i));
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

    int v_star = -1;
    if ((f_in[u] != 0) && (f_out_u != 0))
        v_star = v_max_out_u;

    std::shared_ptr<Raw_Node> current_node;

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
        leaves[v].emplace_back(std::move(current_node), f_x);
    }

    if (v_star != -1)
        trie[v_star]->merge(trie[u], f_max_out_u);

    for (auto &&p : leaves[u])
    {
        std::shared_ptr<Raw_Node> &x = p.first;
        double f_x = p.second;
        double excess = f_x - f_out_u + f_max_out_u;

        if ((v_star == -1) || (excess <= 0))
        {
            std::vector<int> path;
            bool invalid = false;
            current_node = x;
            while (current_node->parent != trie[u]->head)
            {
                path.emplace_back(current_node->value);
                current_node = current_node->parent;
                if (current_node == nullptr)
                {
                    invalid = true;
                    break;
                }
            }
            if ((!invalid) && (!path.empty()))
            {
                path.emplace_back(current_node->value);
                path.emplace_back(u);
                raw_repr.emplace_back(f_x, std::move(path));
            }
        }

        if (v_star != -1)
        {
            f_x = excess;
            while ((f_x <= 0) && (x->children == 0))
            {
                std::shared_ptr<Raw_Node> y = x->parent;
                f_x = f_x + f_in[y->value] - x->flow;
                x->parent.reset();
                y->children--;
                x = y;
            }
            if (x->children == 0)
                leaves[v_star].emplace_back(std::move(x), f_x);
        }
    }

    leaves[u].clear();
    leaves[u].shrink_to_fit();

    return;
}

void Raw::print_maximal_safe_paths()
{
    std::cout << std::fixed << std::setprecision(0);
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

void Raw::topo_dfs(int v, std::vector<bool> &visited)
{
    visited[v] = true;
    for (auto &&u : adjacency_list[v])
    {
        if (!visited[u.first])
            topo_dfs(u.first, visited);
    }
    topo_order.emplace_back(v);
    return;
}
