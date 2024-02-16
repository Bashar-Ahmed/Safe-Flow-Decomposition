#include "main.hpp"
#include "raw.hpp"

MAIN(

    Raw *graph = new Raw(graph_string);
    for (int u : graph->topo_order)
        graph->compute_safe(u);

)
