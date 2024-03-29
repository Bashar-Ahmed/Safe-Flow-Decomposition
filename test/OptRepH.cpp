#include <test.hpp>
#include <optimal.hpp>

class OptRepH : public Base
{
protected:
    void generate_result(std::string graph_string)
    {
        OPTREPH

        for (auto &&path : graph->trivial)
        {
            path.second.push_back(path.second[2]);
            result.push_back(std::move(path));
        }

        for (auto &&paths : graph->optimal_repr)
        {
            for (auto &&path : paths.second)
                result.emplace_back(path.first, std::vector<int>{path.second[0], paths.first.first, paths.first.second, path.second[1]});
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
        delete graph;
    }
};

GENERATE_TESTS(OptRepH)
