#include "../include/optimal.hpp"

int main(int argc, char *argv[])
{
    std::string arg = "profile";
    bool profile = (argc > 1) && (arg.compare(argv[1]) == 0);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Optimal *graph = new Optimal(graph_string);

        graph->compute_non_trivial();
        graph->compute_trivial();

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