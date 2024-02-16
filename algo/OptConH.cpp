#include "main.hpp"
#include "concise.hpp"

MAIN(

    Concise *graph = new Concise(graph_string, true);
    for (int u : graph->topo_order)
        graph->compute_safe(u);

)
