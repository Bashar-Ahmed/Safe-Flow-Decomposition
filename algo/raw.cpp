#include "../include/raw.hpp"

int main(int argc, char *argv[])
{
    std::string arg = "profile";
    bool profile = (argc > 1) && (arg.compare(argv[1]) == 0);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Raw *graph = new Raw(graph_string);

        for (int u : graph->topo_order)
            graph->compute_safe(u);

        if (profile)
            graph->calculate_statistics();
        else
            graph->print_maximal_safe_paths();
        delete graph;
    }
    if (profile)
        Graph::print_statistics();
    return 0;
}