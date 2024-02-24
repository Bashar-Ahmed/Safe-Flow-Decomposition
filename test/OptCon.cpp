#include "utility.hpp"
#include "concise.hpp"

class OptCon : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();

        OPTCON

        for (auto &&path_instance : graph->concise_repr)
        {
            auto it = path_instance.first.begin();
            for (auto &&cut_instance : path_instance.second)
            {
                int u = std::get<0>(cut_instance);
                int v = std::get<1>(cut_instance);
                double flow = std::get<2>(cut_instance);
                while (*it != u)
                    it++;
                std::vector<int> path;
                auto p = it;
                while (*p != v)
                    path.push_back(*p++);
                path.push_back(v);
                result.emplace_back(std::make_pair(flow, std::move(path)));
            }
        }
        delete graph;
    }
};

GENERATE_TESTS(OptCon)
