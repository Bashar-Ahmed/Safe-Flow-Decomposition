#include "main.hpp"
#include "old.hpp"

MAIN(

    Old<RAW> *graph = new Old<RAW>(graph_string, false);
    graph->decompose_path();
    graph->compute_safe();

)
