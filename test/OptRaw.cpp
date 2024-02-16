#include "raw.hpp"
#include "test.hpp"

class OptRaw : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();
        Raw *graph = new Raw(graph_string);
        for (int u : graph->topo_order)
            graph->compute_safe(u);
        result = std::move(graph->raw_repr);
        delete graph;
    }
};

GENERATE_TESTS(OptRaw)
