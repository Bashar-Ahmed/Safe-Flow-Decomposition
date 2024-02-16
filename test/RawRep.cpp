
#include "old.hpp"
#include "test.hpp"

class RawRep : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();
        Old<RAW> *graph = new Old<RAW>(graph_string, false);
        graph->decompose_path();
        graph->compute_safe();

        std::shared_ptr<AC_Trie<double>> root = std::make_shared<AC_Trie<double>>();
        for (auto &&path_instance : graph->raw_repr)
            graph->compress_path(path_instance.first, path_instance.second, root);
        graph->raw_repr.clear();

        root->add_fail();
        std::deque<int> str;
        graph->insert(root, str);

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

GENERATE_TESTS(RawRep)
