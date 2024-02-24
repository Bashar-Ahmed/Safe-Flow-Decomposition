#include <concise.hpp>

template <bool H>
Concise<H>::Concise(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    mark.resize(nodes);
    partial_result.resize(nodes);

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
        trie.emplace_back(std::make_unique<Path_Trie<Concise_Node>>(i));
}

template <bool H>
void Concise<H>::compute_safe(int u)
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

    for (auto &&p : adjacency_list[u])
    {
        int v = p.first;
        if (v == v_star)
            continue;
        double f_x = p.second;

        std::shared_ptr<Concise_Node> current_v, current_u;
        current_u = trie[u]->head;
        int current_u_value = u;
        current_v = trie[v]->insert(u, f_x, trie[v]->head);
        if (v_max_in[v] == u)
            trie[v]->head->v_max_in = current_v;

        while ((current_u->children > 0) && (f_x - f_in[current_u_value] + f_max_in[current_u_value] > 0))
        {
            int k = v_max_in[current_u_value];
            current_v = trie[v]->insert(k, f_max_in[current_u_value], current_v);
            current_v->parent->v_max_in = current_v;
            f_x += f_max_in[current_u_value] - f_in[current_u_value];
            current_u = current_u->v_max_in.lock();
            current_u_value = k;
        }

        mark[current_v->value].emplace_back(v);
        M.emplace_back(current_v->value);
        partial_result[v].emplace_back(std::vector<int>(), std::move(std::vector<Route>(1, std::make_tuple(std::move(current_v), v, f_x))));
    }

    if (v_star != -1)
    {
        trie[v_star]->merge(trie[u], f_max_out_u);
        if (v_max_in[v_star] == u)
            trie[v_star]->head->v_max_in = trie[u]->head;
    }

    for (auto &&p : partial_result[u])
    {

        std::vector<int> &p_k = p.first;
        std::vector<Route> &I_k = p.second;
        Route &last = I_k.back();
        double f_x, f_i = std::get<2>(last);
        std::shared_ptr<Concise_Node> &l_i = std::get<0>(last);
        std::shared_ptr<Concise_Node> x = l_i;

        if (v_star == -1)
            f_x = -std::numeric_limits<double>::max();
        else
            f_x = f_i - f_out_u + f_max_out_u;

        std::vector<int> path;
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
            p_k.insert(p_k.end(), it, path.end());
        }

        if (!p_k.empty() && l_i->value == p_k.back() && x->value == u)
        {
            I_k.pop_back();
            if (!I_k.empty())
            {
                concise_repr.emplace_back(std::move(p_k), std::vector<Cut>());
                std::transform(I_k.begin(), I_k.end(), std::back_inserter(concise_repr.back().second), [](Route &r)
                               { return Cut(std::get<0>(r)->value, std::get<1>(r), std::get<2>(r)); });
            }
            I_k.clear();
            p_k.clear();
        }

        if (f_x > 0 && x->children == 0)
        {
            if (l_i != x)
                I_k.emplace_back(std::move(x), v_star, f_x);
            else
                I_k.back() = {std::move(x), v_star, f_x};
            partial_result[v_star].emplace_back(std::move(p_k), std::move(I_k));
        }
        else if (!mark[x->value].empty())
        {
            int v = mark[x->value].back();
            mark[x->value].pop_back();
            std::vector<Route> &I_v = partial_result[v].back().second;
            I_k.insert(I_k.end(), I_v.begin(), I_v.end());
            partial_result[v].back() = {std::move(p_k), std::move(I_k)};
        }
        else if (!I_k.empty())
        {
            while (p_k.back() != u)
            {
                p_k.emplace_back(x->value);
                x = x->parent;
            }
            concise_repr.emplace_back(std::move(p_k), std::vector<Cut>());
            std::transform(I_k.begin(), I_k.end(), std::back_inserter(concise_repr.back().second), [](Route &r)
                           { return Cut(std::get<0>(r)->value, std::get<1>(r), std::get<2>(r)); });
        }
    }

    for (int x : M)
        mark[x].clear();

    partial_result[u].clear();
    partial_result[u].shrink_to_fit();

    return;
}

template <bool H>
void Concise<H>::print_maximal_safe_paths()
{
    std::cout << metadata << "\n";
    for (auto &path_ind : concise_repr)
    {
        for (auto &value : path_ind.first)
            std::cout << value << " ";
        std::cout << "\n";
        if constexpr (H)
        {
            int start = path_ind.first.front();
            int end = path_ind.first.back();
            int prev_end = -1;
            for (auto &ind : path_ind.second)
            {
                int l = std::get<0>(ind);
                int r = std::get<1>(ind);
                int flow = std::get<2>(ind);
                std::cout << flow << " ";
                if (l != start && l != prev_end)
                    std::cout << l << " ";
                if (r != end)
                    std::cout << r << " ";
                std::cout << "\n";
                prev_end = r;
            }
        }
        else
        {
            for (auto &ind : path_ind.second)
            {
                std::cout << std::get<2>(ind) << " ";
                std::cout << std::get<0>(ind) << " ";
                std::cout << std::get<1>(ind) << " ";
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }
    return;
}

template <bool H>
void Concise<H>::topo_dfs(int v, std::vector<bool> &visited)
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

template class Concise<true>;
template class Concise<false>;
