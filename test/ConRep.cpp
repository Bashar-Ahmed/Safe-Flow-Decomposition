
#include "old.hpp"
#include "test.hpp"

class ConRep : public Base
{
protected:
    void generate_result()
    {
        std::string graph_string = Graph::read();
        Old<CONCISE> *graph = new Old<CONCISE>(graph_string, false);
        graph->decompose_path();
        graph->compute_safe();

        Old<RAW> *dummy = new Old<RAW>(graph_string, false);
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
                std::deque<int> path;
                auto p = it;
                while (*p != v)
                    path.push_back(*p++);
                path.push_back(v);
                dummy->raw_repr.emplace_back(std::make_pair(flow, std::move(path)));
            }
        }
        delete graph;

        std::shared_ptr<AC_Trie<double>> root = std::make_shared<AC_Trie<double>>();
        for (auto &&path_instance : dummy->raw_repr)
            dummy->compress_path(path_instance.first, path_instance.second, root);
        dummy->raw_repr.clear();

        root->add_fail();
        std::deque<int> str;
        dummy->insert(root, str);

        for (auto &&path_instance : dummy->raw_repr)
        {
            std::vector<int> path;
            for (auto &&node : path_instance.second)
                path.push_back(node);
            result.emplace_back(path_instance.first, std::move(path));
        }
        delete dummy;
    }
};

GENERATE_TESTS(ConRep)
