#include "../include/concise.hpp"
#include "../include/debug.hpp"

Concise::Concise(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_out.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    f_max_out.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    v_max_out.resize(nodes, -1);
    trie.resize(nodes);
    partial_result.resize(nodes);
    // mark.resize(nodes);

    for (int i = 0; i < nodes; i++)
    {
        for (auto &edge : adjacency_list[i])
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
            f_in[v] += w;
            f_out[u] += w;
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
        trie[i] = std::make_unique<Path_Trie<ALGO::CONCISE>>(i);
}

void Concise::compute_safe(int u)
{
    for (int i = 0; i < nodes; i++)
    {
        trace(i, partial_result[i]);
    }
    trace(concise_repr);
    int v_star;

    if ((f_in[u] != 0) && (f_out[u] != 0))
        v_star = v_max_out[u];
    else
        v_star = -1;

    std::shared_ptr<Node<ALGO::CONCISE>> current_node_v, current_node_u;

    for (std::pair<int, double> p : adjacency_list[u])
    {
        int v = p.first;
        if (v == v_star)
            continue;
        double f_x = p.second;
        trace(u, v);
        current_node_v = trie[v]->insert(u, f_x, trie[v]->head);

        trie[v]->head->child[u] = current_node_v;
        current_node_u = trie[u]->head;
        int x = current_node_u->value;
        while ((current_node_u->children > 0) && (f_x - f_in[x] + f_max_in[x] > 0))
        {
            int k = v_max_in[x];
            trace(k, current_node_v);
            auto prev_node_v = current_node_v;
            current_node_v = trie[v]->insert(k, f_max_in[x], current_node_v);
            prev_node_v->child[k] = current_node_v;

            f_x += f_max_in[x] - f_in[x];
            current_node_u = current_node_u->child[k];
            x = k;
        }
        partial_result[v].push_back({{}, {{current_node_v, v, f_x}}});
        mark[current_node_v].push_back(v);
    }
    if (v_star != -1)
    {
        trace(u, v_star);
        trie[v_star]->merge(trie[u], f_max_out[u]);
        trie[v_star]->head->child[trie[u]->head->value] = trie[u]->head;
    }
    for (path_index p : partial_result[u])
    {

        std::list<int> p_k = p.first;
        std::list<index> I_k = p.second;
        index last = *I_k.rbegin();
        std::shared_ptr<Node<ALGO::CONCISE>> l_i = std::get<0>(last);
        double f_x, f_i = std::get<2>(last);
        std::shared_ptr<Node<ALGO::CONCISE>> x_node = l_i;
        int x = x_node->value;

        if (v_star == -1)
            f_x = -std::numeric_limits<double>::max();
        else
            f_x = f_i - f_out[u] + f_max_out[u];

        std::list<int> path;
        while ((f_x <= 0) && (mark[x_node].empty()) && (x != u))
        {
            std::shared_ptr<Node<ALGO::CONCISE>> y = x_node->parent_node;
            if (x_node->children <= 0)
            {
                f_x = f_x + f_in[y->value] - x_node->flow_to_parent;
                // x_node->parent_node.reset();
                y->children--;
            }
            else
                break;

            path.push_back(x);
            x = y->value;
            x_node = y;
        }

        auto it = path.begin();
        for (auto &path_element : p_k)
        {
            if ((*it) == path_element)
                it++;
            else
                it = path.begin();
        }
        while (it != path.end())
        {
            p_k.push_back(*it);
            it++;
        }

        if (f_x > 0)
        {
            if (l_i != x_node)
                I_k.push_back({x_node, v_star, f_x});
            else
                I_k.back() = {l_i, v_star, f_x};
            partial_result[v_star].push_back({p_k, I_k});
        }
        else
        {
            if (!mark[x_node].empty())
            {
                int v = mark[x_node].front();
                mark[x_node].pop_front();
                std::list<index> I_v = partial_result[v].back().second;
                partial_result[v].pop_back();
                for (auto &ind : I_v)
                    I_k.push_back(ind);

                partial_result[v].push_back({p_k, I_k});
            }
            else
            {
                if (p_k.back() != x)
                    p_k.push_back(x);
                concise_repr.push_back({p_k, I_k});
            }
        }
    }
    mark.clear();

    return;
}

void Concise::print_maximal_safe_paths()
{
    std::cout << metadata << "\n";
    for (auto &path_ind : concise_repr)
    {
        for (auto &value : path_ind.first)
            std::cout << value << " ";
        std::cout << "\n";
        for (auto &ind : path_ind.second)
        {
            std::cout << "--- " << std::get<0>(ind)->value << " ";
            std::cout << std::get<1>(ind) << " ";
            std::cout << std::get<2>(ind) << " ";
            std::cout << "\n";
        }
    }
    return;
}

void Concise::calculate_statistics()
{
    total_nodes += nodes;
    total_edges += edges;
    for (auto &path_ind : concise_repr)
    {
        length += path_ind.first.size();
        length += path_ind.second.size() * 3;
    }

    return;
}

void Concise::topo_dfs(int v, std::vector<bool> &visited)
{
    visited[v] = true;
    for (std::pair<int, double> u : adjacency_list[v])
    {
        if (!visited[u.first])
            topo_dfs(u.first, visited);
    }
    topo_order.push_back(v);
    return;
}
