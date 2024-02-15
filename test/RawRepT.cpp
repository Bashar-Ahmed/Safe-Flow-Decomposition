
#include "../include/old.hpp"
#include "test.hpp"

class RawRepT : public Base
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
        Old<RAW> *graph = new Old<RAW>(graph_string, true);
        graph->decompose_path();
        graph->compute_safe();

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
