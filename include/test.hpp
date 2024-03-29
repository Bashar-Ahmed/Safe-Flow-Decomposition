#include <utility.hpp>
#include <graph.hpp>

inline std::ifstream Graph::input;

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
        input_file = dataset##_graph;                        \
        truth_file.open(dataset##_truth, std::ifstream::in); \
        run();                                               \
    }

class Base : public ::testing::Test
{
protected:
    int graph_number = -1;
    std::string input_file;
    std::ifstream truth_file;
    std::vector<std::pair<double, std::vector<int>>> truth, result;
    virtual void generate_result(std::string graph_string) = 0;

    void verify()
    {
        std::sort(truth.begin(), truth.end());
        std::sort(result.begin(), result.end());
        bool eq = truth == result;
        SCOPED_TRACE(graph_number);
        EXPECT_TRUE(eq);
        truth.clear();
        result.clear();
        return;
    }

    void run()
    {
        char const *arg[] = {"", input_file.data(), "true", "false"};
        Graph::init(arg);
        bool end = read_truth();
        while (!end)
        {
            graph_number++;
            end = read_truth();
            generate_result(Graph::read());
            verify();
        }
        graph_number++;
        EXPECT_TRUE(Graph::read() == "EOF");
        return;
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
