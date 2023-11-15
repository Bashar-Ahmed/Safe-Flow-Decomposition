#include "../include/complete.hpp"

int main(int argc, char *argv[])
{
    std::string arg = "profile";
    bool profile = (argc > 1) && (arg.compare(argv[1]) == 0);

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        Complete *graph = new Complete(graph_string);

        graph->decompose_path();
        graph->compute_safe();

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