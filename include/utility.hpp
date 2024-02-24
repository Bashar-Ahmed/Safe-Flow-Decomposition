#pragma once

#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

inline std::string zebrafish_graph = "../test/graph.txt";
inline std::string zebrafish_truth = "../test/truth.txt";
inline std::string mouse_graph = "../test/graph.txt";
inline std::string mouse_truth = "../test/truth.txt";
inline std::string human_graph = "../test/graph.txt";
inline std::string human_truth = "../test/truth.txt";
inline std::string salmon_graph = "../test/graph.txt";
inline std::string salmon_truth = "../test/truth.txt";
inline std::string refsim_graph = "../test/graph.txt";
inline std::string refsim_truth = "../test/truth.txt";
inline std::string simulation_graph = "../test/graph.txt";
inline std::string simulation_truth = "../test/truth.txt";

#define RAWREP                                \
    auto *graph = new Old<RAW>(graph_string); \
    graph->decompose_path();                  \
    graph->compute_safe();

#define RAWREPT                                     \
    auto *graph = new Old<RAW, true>(graph_string); \
    graph->decompose_path();                        \
    graph->compute_safe();

#define CONREP                                    \
    auto *graph = new Old<CONCISE>(graph_string); \
    graph->decompose_path();                      \
    graph->compute_safe();

#define CONREPT                                         \
    auto *graph = new Old<CONCISE, true>(graph_string); \
    graph->decompose_path();                            \
    graph->compute_safe();

#define OPTRAW                           \
    auto *graph = new Raw(graph_string); \
    for (int u : graph->topo_order)      \
        graph->compute_safe(u);

#define OPTCON                               \
    auto *graph = new Concise(graph_string); \
    for (int u : graph->topo_order)          \
        graph->compute_safe(u);

#define OPTCONH                                    \
    auto *graph = new Concise<true>(graph_string); \
    for (int u : graph->topo_order)                \
        graph->compute_safe(u);

#define OPTREP                               \
    auto *graph = new Optimal(graph_string); \
    graph->compute_non_trivial();            \
    graph->compute_trivial();

#define OPTREPH                                    \
    auto *graph = new Optimal<true>(graph_string); \
    graph->compute_non_trivial();                  \
    graph->compute_trivial();

#define GENERATE_MAIN(BODY)                           \
    int main()                                        \
    {                                                 \
        Graph::init();                                \
                                                      \
        while (true)                                  \
        {                                             \
            std::string graph_string = Graph::read(); \
            if (graph_string == "")                   \
                break;                                \
                                                      \
            BODY                                      \
                                                      \
                graph->print_maximal_safe_paths();    \
            delete graph;                             \
        }                                             \
        return 0;                                     \
    }

#define GENERATE_TESTS(ALGORITHM)       \
    GENERATE_TEST(ALGORITHM, zebrafish) \
    GENERATE_TEST(ALGORITHM, mouse)     \
    GENERATE_TEST(ALGORITHM, human)     \
    GENERATE_TEST(ALGORITHM, salmon)    \
    GENERATE_TEST(ALGORITHM, refsim)    \
    GENERATE_TEST(ALGORITHM, simulation)

#define GENERATE_TEST(ALGORITHM, dataset)                    \
    TEST_F(ALGORITHM, dataset)                               \
    {                                                        \
        input_file.open(dataset##_graph, std::ifstream::in); \
        truth_file.open(dataset##_truth, std::ifstream::in); \
        int error = run();                                   \
        EXPECT_EQ(error, -1);                                \
    }

class Base : public ::testing::Test
{
protected:
    int graph_number = -1;
    std::ifstream input_file, truth_file;
    std::vector<std::pair<double, std::vector<int>>> truth, result;
    virtual void generate_result() { return; }

    bool verify()
    {
        std::sort(truth.begin(), truth.end());
        std::sort(result.begin(), result.end());
        bool eq = truth == result;
        truth.clear();
        result.clear();
        return eq;
    }

    int run()
    {
        std::cin.rdbuf(input_file.rdbuf());
        read_truth();
        while (true)
        {
            graph_number++;
            bool end = read_truth();
            generate_result();
            if (!verify())
                return graph_number;
            if (end)
                break;
        }
        return -1;
    }

    bool read_truth()
    {
        std::string metadata, line, token;
        while (std::getline(truth_file, line))
        {
            std::pair<double, std::vector<int>> path;
            std::stringstream tokens(line);
            std::getline(tokens, token, ' ');
            if (token == "#")
            {
                std::getline(tokens, metadata);
                return false;
            }
            path.first = stod(token);
            while (std::getline(tokens, token, ' '))
                path.second.push_back(stoi(token));
            truth.push_back(std::move(path));
        }
        return true;
    }
};
