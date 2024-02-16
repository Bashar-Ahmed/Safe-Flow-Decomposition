#include "main.hpp"
#include "optimal.hpp"

MAIN(

    Optimal *graph = new Optimal(graph_string, false);
    graph->compute_non_trivial();
    graph->compute_trivial();

)
