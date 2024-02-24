
#include "utility.hpp"
#include "old.hpp"

class RawRepT : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();

        RAWREPT

        for (auto &&path_instance : graph->raw_repr)
        {
            std::vector<int> path;
            for (auto &&node : path_instance.second)
                path.push_back(node);
            result.emplace_back(path_instance.first, std::move(path));
        }
        delete graph;
    }
};

GENERATE_TESTS(RawRepT)
