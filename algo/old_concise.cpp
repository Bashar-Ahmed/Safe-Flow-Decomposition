#include "../include/old_concise.hpp"
#include <chrono>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int graph_number = 0;

    while (true)
    {
        std::string graph_string = Graph::read();
        if (graph_string == "")
            break;

        // long long total_time = 0;
        // int iterations = 0;
        // while (true)
        // {
        //     iterations++;
        //     auto start = std::chrono::high_resolution_clock::now();

        Old_Concise *graph = new Old_Concise(graph_string);

        graph->decompose_path();
        graph->compute_safe();
        graph->print_maximal_safe_paths();
        delete graph;

        // auto end = std::chrono::high_resolution_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // total_time += elapsed.count();
        // if (total_time >= 1000000)
        // {
        //     std::cerr << graph_number << " " << (total_time / iterations) << "\n";
        //     break;
        // }
        // }
        // graph_number++;
    }
    return 0;
}