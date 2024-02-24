#include <utility.hpp>

#define GENERATE_MAIN(BODY)                           \
    int main()                                        \
    {                                                 \
        Graph::init();                                \
                                                      \
        while (true)                                  \
        {                                             \
            std::string graph_string = Graph::read(); \
            if (graph_string == "EOF")                   \
                break;                                \
                                                      \
            BODY                                      \
                                                      \
                graph->print_maximal_safe_paths();    \
            delete graph;                             \
        }                                             \
        return 0;                                     \
    }
