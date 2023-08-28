#include "../include/concise.hpp"
#include "../include/debug.hpp"

Concise::Concise(const std::string &graph) : Graph(graph)
{

    f_max_in.resize(nodes, 0);
    f_max_out.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    v_max_out.resize(nodes, -1);
    trie.resize(nodes);
    partial_result.resize(nodes);
    mark.resize(nodes);

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
    int v_star;
    std::list<int> M;

    if ((f_in[u] != 0) && (f_out[u] != 0))
        v_star = v_max_out[u];
    else
        v_star = -1;

    std::shared_ptr<Node> current_node_v, current_node_u;

    for (std::pair<int, double> p : adjacency_list[u])
    {
        int v = p.first;
        if (v == v_star)
            continue;
        double f_x = p.second;

        trace(v, u);

        current_node_v = trie[v]->insert(u, f_x, trie[v]->head);
        current_node_u = trie[u]->head;
        int x = current_node_u->value;

        while ((current_node_u->children > 0) && (f_x - f_in[x] + f_max_in[x] > 0))
        {
            int k = v_max_in[x];
            trace(v, k);

            current_node_v = trie[v]->insert(k, f_max_in[x], current_node_v);
            f_x += f_max_in[x] - f_in[x];
            current_node_u = current_node_u->child[k];
            x = k;
        }
        trace(current_node_v.get(), current_node_v->value, f_x);
        partial_result[v].push_back({{}, {{current_node_v, v, f_x}}});
        mark[x].push(v);
        M.push_back(x);
    }

    if (v_star != -1)
    {
        trace(v_star, u);
        trie[v_star]->insert(trie[u], f_max_out[u], trie[v_star]->head);
    }

    for (path_index p : partial_result[u])
    {
        // trace(u, p);
        trace(p);
        std::list<int> p_k = p.first;
        std::list<index> I_k = p.second;
        index last = I_k.back();
        std::shared_ptr<Node> l_i = std::get<0>(last);
        double f_x, f_i = std::get<2>(last);
        std::shared_ptr<Node> x_node = l_i;
        int x = x_node->value;

        if (v_star == -1)
            f_x = -std::numeric_limits<double>::max();
        else
            f_x = f_i - f_out[u] + f_max_out[u];

        std::list<int> path;
        while ((f_x <= 0) && (mark[x].empty()) && (x != u))
        {
            trace(u, x_node->value, x_node.get());
            std::shared_ptr<Node> y = x_node->parent_node;
            if (y->children > 1)
            {
                trace(x, y->value);
                f_x = f_x + f_in[y->value] - x_node->flow_to_parent;
                // x_node->parent_node.reset();
                y->children--;
                y->child.erase(x);
            }

            path.push_back(x);
            x = y->value;
            x_node = y;
        }
        // trace(path);
        // path.push_back(x);
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
        // trace(p_k, path);
        // if (x_node->parent_node == nullptr)
        //     trace(path, f_x, mark[x], I_k);
        if (f_x > 0)

        {
            trace(x_node.get(), l_i.get());
            trace(x_node->value, v_star, f_x);
            if (l_i != x_node)
                I_k.push_back({x_node, v_star, f_x});
            else
            {
                I_k.pop_back();
                I_k.push_back({l_i, v_star, f_x});
            }
            trace(I_k);
            partial_result[v_star].push_back({p_k, I_k});
        }
        else
        {
            if (!mark[x].empty())
            {
                trace(mark[x]);

                int v = mark[x].front();
                mark[x].pop();
                std::list<index> I_v = partial_result[v].back().second;
                partial_result[v].pop_back();
                for (auto &ind : I_k)
                    I_v.push_back(ind);
                trace(I_v);

                partial_result[v].push_back({p_k, I_v});
            }
            else
            {
                if (p_k.back() != x)
                    p_k.push_back(x);
                concise_repr.push_back({p_k, I_k});
            }
        }
    }
    for (auto &x : M)
    {
        std::queue<int> f;
        mark[x] = f;
    }
    return;
}

void Concise::print_maximal_safe_paths()
{
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
