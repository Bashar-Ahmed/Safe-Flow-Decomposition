#pragma once

#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

inline std::string zebrafish_graph = "../test/test.graph";
inline std::string zebrafish_truth = "../test/test.truth";
inline std::string mouse_graph = "../test/test.graph";
inline std::string mouse_truth = "../test/test.truth";
inline std::string human_graph = "../test/test.graph";
inline std::string human_truth = "../test/test.truth";
inline std::string salmon_graph = "../test/test.graph";
inline std::string salmon_truth = "../test/test.truth";
inline std::string refsim_graph = "../test/test.graph";
inline std::string refsim_truth = "../test/test.truth";
inline std::string simulation_graph = "../test/test.graph";
inline std::string simulation_truth = "../test/test.truth";

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
