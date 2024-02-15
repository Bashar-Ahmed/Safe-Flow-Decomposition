#include "../include/raw.hpp"
#include "test.hpp"

class OptRaw : public Base
{
protected:
    bool verify()
    {
        std::sort(truth.begin(), truth.end());
        std::sort(result.begin(), result.end());
        bool eq = truth == result;
        truth.clear();
        result.clear();
        return eq;
    }
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
