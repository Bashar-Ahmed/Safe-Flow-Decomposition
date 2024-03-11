#include <utility.hpp>

#define GENERATE_MAIN(BODY)                           \
    std::ifstream Graph::input;                       \
    int main(int argc, char const *argv[])            \
    {                                                 \
        if (argc == 1)                                \
        {                                             \
            std::cout << "NO INPUT FILE SPECIFIED\n"; \
            return 0;                                 \
        }                                             \
        Graph::init(argv);                            \
        while (true)                                  \
        {                                             \
            std::string graph_string = Graph::read(); \
            if (graph_string == "EOF")                \
                break;                                \
            BODY;                                     \
            delete graph;                             \
        }                                             \
        return 0;                                     \
    }
