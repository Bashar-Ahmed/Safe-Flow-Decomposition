#include "../include/io.hpp"
#include "../include/complete.hpp"

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    while(true) {
        std::pair<std::string,bool> status = read_graph();
        std::string graph_string = status.first;
        if(graph_string=="\n") continue;

        Complete* graph = new Complete(graph_string);

        graph->decompose_path();
        graph->compute_safe();
        graph->compress_path();
        
        std::cout<<graph->metadata<<"\n";
        graph->print_complete_decomposition();
        delete graph;
        if(!status.second) break;
    }
    return 0;
}