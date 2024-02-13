#include "../include/old.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Old *graph = new Old(graph_string, false, true);

        graph->decompose_path();
        graph->compute_safe();
        graph->print_maximal_safe_paths();
        delete graph;
    }
    return 0;
}