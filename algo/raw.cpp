#include "../include/io.hpp"
#include "../include/raw.hpp"

int main()
{

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while (true)
    {
        std::pair<std::string, bool> status = read_graph();
        std::string graph_string = status.first;
        if (graph_string == "\n")
            continue;

        Raw *graph = new Raw(graph_string);

        for (int u : graph->topo_order)
        {
            graph->compute_safe(u);
        }

        std::cout << graph->metadata << "\n";
        graph->print_maximal_safe_paths();
        delete graph;
        if (!status.second)
            break;
    }
    return 0;
}