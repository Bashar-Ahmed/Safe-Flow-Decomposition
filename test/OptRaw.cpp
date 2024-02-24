#include "utility.hpp"
#include "raw.hpp"

class OptRaw : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();

        OPTRAW

        result = std::move(graph->raw_repr);
        delete graph;
    }
};

GENERATE_TESTS(OptRaw)
