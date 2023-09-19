#include "../include/graph.hpp"

Graph::Graph(const std::string &graph)
{
    std::stringstream lines(graph);
    std::string temp;

    int line = 0;
    while (std::getline(lines, temp, '\n'))
    {
        if (line == 0)
            metadata = "#" + temp;
        else if (line == 1)
        {
            nodes = stoi(temp);
            adjacency_list.resize(nodes);
        }
        else
        {
            std::stringstream tokens(temp);
            int token = 0;
            int u, v;
            double w;
            while (std::getline(tokens, temp, ' '))
            {
                if (token == 0)
                    u = stoi(temp);
                else if (token == 1)
                    v = stoi(temp);
                else
                    w = stod(temp);
                token++;
            }
            adjacency_list[u].push_back({v, w});
        }
        line++;
    }
    edges = line - 2;
}

void Graph::print_statistics()
{
    std::cout << "Nodes: " << total_nodes << "\n";
    std::cout << "Edges: " << total_edges << "\n";
    std::cout << "Tokens: " << length << "\n";
}
