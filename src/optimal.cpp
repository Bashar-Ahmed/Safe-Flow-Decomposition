#include <optimal.hpp>

template <bool H>
Optimal<H>::Optimal(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_out.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    f_max_out.resize(nodes, 0);
    v_max_in.resize(nodes, -1);
    v_max_out.resize(nodes, -1);
    c_max_in.resize(nodes, 0);
    c_max_out.resize(nodes, 0);

    forest_in.reserve(nodes);
    forest_out.reserve(nodes);

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
                c_max_in[v] = 1;
            }
            else if (f_max_in[v] == w)
                c_max_in[v]++;

            if (f_max_out[u] < w)
            {
                f_max_out[u] = w;
                v_max_out[u] = v;
                c_max_out[u] = 1;
            }
            else if (f_max_out[u] == w)
                c_max_out[u]++;

            f_in[v] += w;
            f_out[u] += w;
        }
    }

    construct_forest();

    if (print)
        std::cout << metadata << "\n";
}
template <bool H>
Optimal<H>::~Optimal()
{
    if (store && print)
        print_safe_paths();
}

template <bool H>
int Optimal<H>::binary_search_1(Forest_Node *node, double flow)
{
    int l = 0;
    int r = node->depth;
    double loss = node->loss;
    while (l < r)
    {
        int m = (l + r) >> 1;
        if (abs(loss - node->level_ancestor(m)->loss) < flow)
            r = m;
        else
            l = m + 1;
    }
    return node->level_ancestor(r)->value;
}

template <bool H>
int Optimal<H>::binary_search_2(Forest_Node *node_1, Forest_Node *node_2)
{
    int l = node_2->depth + 1;
    int r = node_1->depth;
    double loss = node_2->loss;
    while (l < r)
    {
        int m = (l + r + 1) >> 1;
        auto z = node_1->level_ancestor(m);
        if (z->flow > loss - forest_in[z->parent]->loss)
            l = m;
        else
            r = m - 1;
    }
    return node_1->level_ancestor(l)->value;
}

template <bool H>
void Optimal<H>::compute_non_trivial()
{
    for (int i = 0; i < nodes; i++)
    {
        for (auto &&p : adjacency_list[i])
        {
            int j = p.first;
            if (forest_in[j]->parent == i)
                continue;
            bool new_edge = true;
            std::vector<std::pair<double, std::vector<int>>> paths;

            double excess_flow = p.second;
            double i_loss = forest_in[i]->loss;
            double j_loss = forest_out[j]->loss;

            while (excess_flow > 0)
            {
                int l = binary_search_1(forest_in[i].get(), excess_flow);
                double in_loss = forest_in[l]->loss - i_loss;
                double rest_flow = p.second - abs(in_loss);
                int r = binary_search_1(forest_out[j].get(), rest_flow);
                if (l != i || r != j)
                {
                    double flow = p.second - in_loss - (forest_out[r]->loss - j_loss);
                    if (!left_extendible(l, flow) && !right_extendible(r, flow))
                    {
                        if (new_edge)
                        {
                            new_edge = false;
                            if (store)
                                optimal_repr.emplace_back(std::make_pair(i, j), std::vector<std::pair<double, std::vector<int>>>());
                        }
                        if (store)
                            optimal_repr.back().second.emplace_back(flow, std::vector<int>{l, r});
                        else if (print)
                            paths.emplace_back(flow, std::vector<int>{l, r});
                    }
                }
                int next = forest_out[r]->parent;
                if (forest_out[r]->parent == r)
                    break;
                excess_flow = p.second - abs(forest_out[next]->loss - j_loss);
            }
            if (!paths.empty())
                print_safe_path(std::make_pair(i, j), paths);
        }
    }
}

template <bool H>
bool Optimal<H>::left_extendible(int node, double flow)
{
    if (f_in[node] == 0)
        return false;
    return flow > f_in[node] - f_max_in[node];
}

template <bool H>
bool Optimal<H>::right_extendible(int node, double flow)
{
    if (f_out[node] == 0)
        return false;
    return flow > f_out[node] - f_max_out[node];
}

template <bool H>
void Optimal<H>::compute_trivial()
{
    std::vector<bool> visited(nodes, false);
    for (int i = 0; i < nodes; i++)
    {
        auto x = forest_in[i];
        int x_value = x->value;
        int x_parent = x->parent;

        if (x->children.size() > 0)
            continue;

        auto r = x->level_ancestor(0);
        int r_value = r->value;
        double r_loss = r->loss;

        while (x_parent != x_value)
        {
            if (visited[x_value])
                break;
            visited[x_value] = true;
            double flow = x->flow;
            double loss = r_loss - forest_in[x_parent]->loss;
            if (loss < flow)
            {
                if ((!right_extendible(x_value, flow - loss)))
                {
                    if (r_value != x_parent && (!left_extendible(r_value, flow - loss)))
                    {
                        if (store)
                            trivial.emplace_back(flow - loss, std::vector<int>{r_value, x_parent, x_value});
                        else if (print)
                            print_safe_path(flow - loss, std::vector<int>{r_value, x_parent, x_value});
                    }
                }
                break;
            }
            int y = binary_search_1(forest_in[x_parent].get(), flow);
            loss = forest_in[y]->loss - forest_in[x_parent]->loss;
            if (!right_extendible(x_value, flow - loss))
            {
                if (y != x_parent && (!left_extendible(y, flow - loss)))
                {
                    if (store)
                        trivial.emplace_back(flow - loss, std::vector<int>{y, x_parent, x_value});
                    else if (print)
                        print_safe_path(flow - loss, std::vector<int>{y, x_parent, x_value});
                }
            }
            y = forest_in[y]->parent;
            x = forest_in[binary_search_2(x.get(), forest_in[y].get())];
            x_value = x->value;
            x_parent = x->parent;
        }
    }
}

template <bool H>
void Optimal<H>::print_safe_path(double flow, std::vector<int> path)
{
    std::cout << flow << " ";
    std::cout << path[0] << " ";
    if constexpr (!H)
        std::cout << path[1] << " ";
    std::cout << path[2];
    std::cout << "\n";
    return;
}

template <bool H>
void Optimal<H>::print_safe_path(std::pair<int, int> edge, std::vector<std::pair<double, std::vector<int>>> &paths)
{
    std::cout << edge.first << " " << edge.second << "\n";
    for (auto &&path : paths)
    {
        std::cout << path.first << " ";
        if constexpr (H)
        {
            if (path.second[0] != edge.first)
                std::cout << path.second[0] << " ";
            if (path.second[1] != edge.second)
                std::cout << path.second[1];
        }
        else
        {
            std::cout << path.second[0] << " ";
            std::cout << path.second[1];
        }
        std::cout << "\n";
    }
    return;
}

template <bool H>
void Optimal<H>::print_safe_paths()
{
    for (auto &&path : optimal_repr)
        print_safe_path(path.first, path.second);
    for (auto &&path : trivial)
        print_safe_path(path.first, path.second);
    return;
}

template <bool H>
void Optimal<H>::construct_forest()
{
    for (int i = 0; i < nodes; i++)
    {
        forest_in.emplace_back(std::make_shared<Forest_Node>(i));
        forest_out.emplace_back(std::make_shared<Forest_Node>(i));
    }

    for (int i = 0; i < nodes; i++)
    {
        if (c_max_in[i] == 1)
        {
            forest_in[v_max_in[i]]->children.emplace_back(i);
            forest_in[i]->parent = v_max_in[i];
            forest_in[i]->flow = f_max_in[i];
        }
        if (c_max_out[i] == 1)
        {
            forest_out[v_max_out[i]]->children.emplace_back(i);
            forest_out[i]->parent = v_max_out[i];
            forest_out[i]->flow = f_max_out[i];
        }
    }

    for (int i = 0; i < nodes; i++)
    {
        if (forest_in[i]->parent == i)
        {
            forest_in[i]->init_root();
            int label = 0;
            dfs_in(i, label);

            for (auto &&depth : *forest_in[i]->depth_map)
                std::sort(depth.begin(), depth.end());
        }
        if (forest_out[i]->parent == i)
        {
            forest_out[i]->init_root();
            int label = 0;
            dfs_out(i, label);

            for (auto &&depth : *forest_out[i]->depth_map)
                std::sort(depth.begin(), depth.end());
        }
    }
}

template <bool H>
void Optimal<H>::dfs_in(int n, int &label)
{
    forest_in[n]->init_node(label);
    label++;

    for (auto &&child : forest_in[n]->children)
    {
        forest_in[child]->update_node(forest_in[n].get(), f_max_in[child] - f_in[child]);
        dfs_in(child, label);
    }
}

template <bool H>
void Optimal<H>::dfs_out(int n, int &label)
{
    forest_out[n]->init_node(label);
    label++;

    for (auto &&child : forest_out[n]->children)
    {
        forest_out[child]->update_node(forest_out[n].get(), f_max_out[child] - f_out[child]);
        dfs_out(child, label);
    }
}

template class Optimal<true>;
template class Optimal<false>;
