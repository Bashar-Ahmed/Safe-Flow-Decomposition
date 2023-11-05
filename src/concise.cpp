#include "../include/concise.hpp"

Concise::Concise(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    mark.resize(nodes);
    trie.resize(nodes);
    partial_result.resize(nodes);

    for (int i = 0; i < nodes; i++)
    {
        mark[i].reserve(100);
        partial_result[i].reserve(100);

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
        trie[i] = std::make_unique<Path_Trie<Concise_Node>>(i);
}

void Concise::compute_safe(int u)
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

    std::vector<int> M;
    M.reserve(100);

    for (auto &&p : adjacency_list[u])
    {
        int v = p.first;
        if (v == v_star)
            continue;
        double f_x = p.second;

        std::shared_ptr<Concise_Node> current_node_v, current_node_u;
        current_node_u = trie[u]->head;
        current_node_v = trie[v]->insert(u, f_x, trie[v]->head);
        if (v_max_in[v] == u)
            trie[v]->head->v_max_in = current_node_v;

        while ((current_node_u->children > 0) && (f_x - f_in[current_node_u->value] + f_max_in[current_node_u->value] > 0))
        {
            int k = v_max_in[current_node_u->value];
            current_node_v = trie[v]->insert(k, f_max_in[current_node_u->value], current_node_v);
            current_node_v->parent->v_max_in = current_node_v;
            f_x += f_max_in[current_node_u->value] - f_in[current_node_u->value];
            current_node_u = current_node_u->v_max_in;
        }

        mark[current_node_v->value].emplace_back(v);
        M.emplace_back(current_node_v->value);
        partial_result[v].emplace_back(std::list<int>(), std::move(std::list<cut>(1, std::make_tuple(std::move(current_node_v), v, f_x))));
    }

    if (v_star != -1)
    {
        trie[v_star]->merge(trie[u], f_max_out_u);
        if (v_max_in[v_star] == u)
            trie[v_star]->head->v_max_in = trie[u]->head;
    }

    for (auto &&p : partial_result[u])
    {

        std::list<int> &p_k = p.first;
        std::list<cut> &I_k = p.second;
        cut &last = I_k.back();
        double f_x, f_i = std::get<2>(last);
        std::shared_ptr<Concise_Node> &l_i = std::get<0>(last);
        std::shared_ptr<Concise_Node> x = l_i;

        if (v_star == -1)
            f_x = -std::numeric_limits<double>::max();
        else
            f_x = f_i - f_out_u + f_max_out_u;

        std::list<int> path;
        while ((f_x <= 0) && (mark[x->value].empty()) && (x->value != u))
        {
            std::shared_ptr<Concise_Node> y = x->parent;
            if (x->children == 0)
            {
                f_x = f_x + f_in[y->value] - x->flow;
                x->parent.reset();
                y->children--;
            }
            else
                break;
            path.emplace_back(x->value);
            x = y;
        }

        if (!path.empty())
        {
            auto it = path.begin();
            for (auto &&path_element : p_k)
            {
                if (*it == path_element)
                    it++;
                else
                    it = path.begin();
            }
            path.splice(path.begin(), p_k);
            std::swap(path, p_k);
        }

        if (f_x > 0 && x->children == 0)
        {
            if (l_i != x)
                I_k.emplace_back(std::move(x), v_star, f_x);
            else
                I_k.back() = {std::move(l_i), v_star, f_x};
            partial_result[v_star].emplace_back(std::move(p_k), std::move(I_k));
        }
        else if (!mark[x->value].empty())
        {
            int v = mark[x->value].back();
            mark[x->value].pop_back();
            std::list<cut> &I_v = partial_result[v].back().second;
            I_k.splice(I_k.end(), I_v);
            partial_result[v].back() = {std::move(p_k), std::move(I_k)};
        }
        else
        {
            int end = std::get<1>(I_k.back());
            while (p_k.back() != end)
            {
                p_k.emplace_back(x->value);
                x = x->parent;
            }

            auto it = p_k.begin();
            auto itl = I_k.begin();
            auto itr = I_k.begin();
            auto I_k_end = I_k.end();
            auto p_k_end = p_k.end();
            int current = 0;
            std::list<int> p;
            std::list<cut> I;

            while (it != p_k_end)
            {
                if (itl != I_k_end && *it == std::get<0>(*itl)->value)
                {
                    if (*++it != std::get<1>(*itl))
                    {
                        I.emplace_back(*itl);
                        current++;
                    }
                    it--;
                    itl++;
                }

                if (itr != I_k_end && *it == std::get<1>(*itr))
                {
                    if (*--it != std::get<0>(*itr)->value)
                        current--;
                    it++;
                    itr++;
                }

                p.emplace_back(*it++);

                if (current == 0)
                {
                    if (!I.empty())
                        concise_repr.emplace_back(std::move(p), std::move(I));
                    else
                        p.clear();
                }
            }
        }
    }

    for (int x : M)
        mark[x].clear();

    return;
}

void Concise::print_maximal_safe_paths()
{
    std::cout << metadata << "\n\n";
    for (auto &path_ind : concise_repr)
    {
        for (auto &value : path_ind.first)
            std::cout << value << " ";
        std::cout << "\n";
        for (auto &ind : path_ind.second)
        {
            std::cout << std::get<0>(ind)->value << " ";
            std::cout << std::get<1>(ind) << " ";
            std::cout << std::get<2>(ind) << " ";
            std::cout << "\n";
        }
        std::cout << "\n";
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
    topo_order.emplace_back(v);
    return;
}
