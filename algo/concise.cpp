#include "../include/concise.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Concise *graph = new Concise(graph_string);

        for (int u : graph->topo_order)
            graph->compute_safe(u);
        graph->print_maximal_safe_paths();
        delete graph;
    }
    return 0;
}