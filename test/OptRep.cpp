#include "../include/optimal.hpp"
#include "test.hpp"

class OptRep : public Base
{
protected:
    bool verify() { return truth == result; }
    void generate_result()
    {
        std::string graph_string = Graph::read();
        Optimal *graph = new Optimal(graph_string, false);
        graph->compute_non_trivial();
        graph->compute_trivial();
        result = std::move(graph->optimal_repr);

        for (auto &&path : graph->trivial)
        {
            path.second.push_back(path.second[2]);
            result.push_back(std::move(path));
        }

        for (auto &&path : result)
        {
            std::vector<int> complete_path;
            int l = path.second[0];
            int i = path.second[1];
            int j = path.second[2];
            int r = path.second[3];

            complete_path.push_back(i);
            while (i != l)
            {
                i = graph->forest_in[i]->parent;
                complete_path.push_back(i);
            }
            std::reverse(complete_path.begin(), complete_path.end());
            complete_path.push_back(j);
            while (j != r)
            {
                j = graph->forest_out[j]->parent;
                complete_path.push_back(j);
            }
            path.second = std::move(complete_path);
        }

        std::sort(result.begin(), result.end());
        delete graph;
    }
};

GENERATE_TESTS(OptRep)
