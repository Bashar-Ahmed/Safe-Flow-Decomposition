#include "../include/optimal.hpp"
#include "../include/debug.hpp"

Optimal::Optimal(const std::string &graph) : Graph(graph)
{
    f_in.resize(nodes, 0);
    f_out.resize(nodes, 0);
    f_max_in.resize(nodes, 0);
    f_max_out.resize(nodes, 0);
    v_max_in.resize(nodes, {});
    v_max_out.resize(nodes, {});

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
                v_max_in[v] = {u};
            }
            else if (f_max_in[v] == w)
                v_max_in[v].push_back(u);

            if (f_max_out[u] < w)
            {
                f_max_out[u] = w;
                v_max_out[u] = {v};
            }
            else if (f_max_out[u] == w)
                v_max_out[u].push_back(v);

            f_in[v] += w;
            f_out[u] += w;
        }
    }

    for (auto &&v : v_max_in)
        if (v.size() > 1)
            v.clear();
    for (auto &&v : v_max_out)
        if (v.size() > 1)
            v.clear();

    construct_forest();
}

int Optimal::binary_search_1(std::shared_ptr<Optimal_Node> node, double flow)
{
    int l = 0;
    int r = node->depth;
    while (l < r)
    {
        int m = (l + r) / 2;
        auto y = node->level_ancestor(m);
        if (abs(node->loss - y->loss) < flow)
            r = m;
        else
            l = m + 1;
    }
    return node->level_ancestor(r)->value;
}

int Optimal::binary_search_2(std::shared_ptr<Optimal_Node> node_1, std::shared_ptr<Optimal_Node> node_2)
{
    int l = node_2->depth + 1;
    int r = node_1->depth;
    while (l < r)
    {
        int m = (l + r + 1) / 2;
        auto z = node_1->level_ancestor(m);
        if (z->flow > node_2->loss - forest_in[z->parent]->loss)
            l = m;
        else
            r = m - 1;
    }
    return node_1->level_ancestor(l)->value;
}

void Optimal::compute_non_trivial()
{
    for (int i = 0; i < nodes; i++)
    {
        for (auto &&p : adjacency_list[i])
        {
            int j = p.first;
            if (forest_in[j]->parent == i)
                continue;

            double excess_flow = p.second;

            while (excess_flow > 0)
            {
                int l = binary_search_1(forest_in[i], excess_flow);
                double rest_flow = excess_flow - abs(forest_in[i]->loss - forest_in[l]->loss);
                int r = binary_search_1(forest_out[j], rest_flow);
                if (l != i || r != j)
                {
                    double flow = p.second - (forest_in[l]->loss - forest_in[i]->loss) - (forest_out[r]->loss - forest_out[j]->loss);
                    optimal_repr.push_back({flow, {l, i, j, r}});
                    break;
                }
                int next = forest_out[r]->parent;
                if (forest_out[next]->parent == next)
                    break;
                excess_flow = p.second - abs(forest_out[next]->loss - forest_out[j]->loss);
            }
        }
    }
}

bool Optimal::left_extendible(int node, double flow)
{
    if (f_in[node] == 0)
        return false;
    return flow > f_in[node] - f_max_in[node];
}

bool Optimal::right_extendible(int node, double flow)
{
    if (f_out[node] == 0)
        return false;
    return flow > f_out[node] - f_max_out[node];
}

void Optimal::compute_trivial()
{
    std::vector<bool> visited(nodes, false);
    for (int i = 0; i < nodes; i++)
    {
        auto x = forest_in[i];
        if (x->children.size() > 0)
            continue;

        auto r = x->level_ancestor(0);
        while (x->parent != x->value)
        {
            if (visited[x->value])
                break;
            visited[x->value] = true;
            double flow = x->flow;
            double loss = r->loss - forest_in[x->parent]->loss;
            if (loss < flow)
            {
                if ((!right_extendible(x->value, flow - loss)) && (!left_extendible(r->value, flow - loss)))
                {
                    if (r->value != x->parent)
                        optimal_repr.push_back({flow - loss, {r->value, x->parent, x->value}});
                }
                break;
            }
            auto y = binary_search_1(forest_in[x->parent], flow);
            loss = forest_in[y]->loss - forest_in[x->parent]->loss;
            if (!right_extendible(x->value, flow - loss))
            {
                if (y != x->parent)
                    optimal_repr.push_back({flow - loss, {y, x->parent, x->value}});
            }
            y = forest_in[y]->parent;
            x = forest_in[binary_search_2(x, forest_in[y])];
        }
    }
}

void Optimal::print_maximal_safe_paths()
{
    std::cout << metadata << "\n";
    for (auto &&path : optimal_repr)
    {
        std::cout << path.first << " ";
        for (auto &&value : path.second)
            std::cout << value << " ";
        std::cout << "\n";
    }
    return;
}

void Optimal::calculate_statistics()
{
    total_nodes += nodes;
    total_edges += edges;
    for (auto &&path : optimal_repr)
        length += path.second.size() + 1;
    return;
}

void Optimal::construct_forest()
{
    for (int i = 0; i < nodes; i++)
    {
        forest_in.emplace_back(std::move(std::make_shared<Optimal_Node>(i)));
        forest_out.emplace_back(std::move(std::make_shared<Optimal_Node>(i)));
    }

    for (int i = 0; i < nodes; i++)
    {
        if (!v_max_in[i].empty())
        {
            forest_in[v_max_in[i][0]]->children.push_back(i);
            forest_in[i]->parent = v_max_in[i][0];
            forest_in[i]->flow = f_max_in[i];
        }
        if (!v_max_out[i].empty())
        {
            forest_out[v_max_out[i][0]]->children.push_back(i);
            forest_out[i]->parent = v_max_out[i][0];
            forest_out[i]->flow = f_max_in[i];
        }
    }

    for (int i = 0; i < nodes; i++)
    {
        if (forest_in[i]->parent == i)
        {
            forest_in[i]->loss = 0;
            forest_in[i]->depth = 0;
            forest_in[i]->depth_map = std::make_shared<std::vector<std::vector<int>>>();
            forest_in[i]->label_map = std::make_shared<std::vector<std::shared_ptr<Optimal_Node>>>();

            int label = 0;
            dfs_in(i, label);

            for (auto &&depth : *forest_in[i]->depth_map)
                std::sort(depth.begin(), depth.end());
        }
        if (forest_out[i]->parent == i)
        {
            forest_out[i]->loss = 0;
            forest_out[i]->depth = 0;
            forest_out[i]->depth_map = std::make_shared<std::vector<std::vector<int>>>();
            forest_out[i]->label_map = std::make_shared<std::vector<std::shared_ptr<Optimal_Node>>>();

            int label = 0;
            dfs_out(i, label);

            for (auto &&depth : *forest_out[i]->depth_map)
                std::sort(depth.begin(), depth.end());
        }
    }
}

void Optimal::dfs_in(int n, int &label)
{
    if (int(forest_in[n]->depth_map->size()) == forest_in[n]->depth)
        forest_in[n]->depth_map->push_back({});
    forest_in[n]->depth_map->operator[](forest_in[n]->depth).push_back(label);
    forest_in[n]->label_map->push_back(forest_in[n]);
    forest_in[n]->label = label;
    label++;

    for (auto &&child : forest_in[n]->children)
    {
        forest_in[child]->loss = forest_in[n]->loss + f_max_in[child] - f_in[child];
        forest_in[child]->depth = forest_in[n]->depth + 1;
        forest_in[child]->depth_map = forest_in[n]->depth_map;
        forest_in[child]->label_map = forest_in[n]->label_map;
        dfs_in(child, label);
    }
}

void Optimal::dfs_out(int n, int &label)
{
    if (int(forest_out[n]->depth_map->size()) == forest_out[n]->depth)
        forest_out[n]->depth_map->push_back({});
    forest_out[n]->depth_map->operator[](forest_out[n]->depth).push_back(label);
    forest_out[n]->label_map->push_back(forest_out[n]);
    forest_out[n]->label = label;
    label++;

    for (auto &&child : forest_out[n]->children)
    {
        forest_out[child]->loss = forest_out[n]->loss + f_max_out[child] - f_out[child];
        forest_out[child]->depth = forest_out[n]->depth + 1;
        forest_out[child]->depth_map = forest_out[n]->depth_map;
        forest_out[child]->label_map = forest_out[n]->label_map;
        dfs_out(child, label);
    }
}
