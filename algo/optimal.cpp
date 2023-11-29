#include "../include/optimal.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Optimal *graph = new Optimal(graph_string);

        graph->compute_non_trivial();
        graph->compute_trivial();
        graph->print_maximal_safe_paths();
        delete graph;
    }
    return 0;
}