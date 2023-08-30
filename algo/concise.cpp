#include "../include/io.hpp"
#include "../include/concise.hpp"

int main(int argc, char *argv[])
{
    std::string arg = "profile";
    bool profile = (argc > 1) && (arg.compare(argv[1]) == 0);
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while (true)
    {
        std::pair<std::string, bool> status = read_graph();
        std::string graph_string = status.first;
        if (graph_string == "\n")
            continue;

        Concise *graph = new Concise(graph_string);

        for (int u : graph->topo_order)
        {
            graph->compute_safe(u);
        }

        if (profile)
            graph->calculate_statistics();
        else
            graph->print_maximal_safe_paths();
        delete graph;
        if (!status.second)
            break;
    }
    if (profile)
        Graph::print_statistics();
    return 0;
}