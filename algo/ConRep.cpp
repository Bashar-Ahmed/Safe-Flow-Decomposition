#include "main.hpp"
#include "old.hpp"

MAIN(

    Old<CONCISE> *graph = new Old<CONCISE>(graph_string, false);
    graph->decompose_path();
    graph->compute_safe();

)
