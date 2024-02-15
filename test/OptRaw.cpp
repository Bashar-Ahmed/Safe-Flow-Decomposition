//         }
//     }
//     void generate_result()
//     {
//         std::cin.rdbuf(input_file.rdbuf());

//

#include "../include/raw.hpp"
#include "test.hpp"

class OptRaw : public Base
{
protected:
    bool verify() { return truth == result; }
    void generate_result()
    {
        std::string graph_string = Graph::read();
        Raw *graph = new Raw(graph_string);
        for (int u : graph->topo_order)
            graph->compute_safe(u);
        result = std::move(graph->raw_repr);

        std::sort(result.begin(), result.end());
        delete graph;
    }
};

GENERATE_TESTS(OptRaw)
