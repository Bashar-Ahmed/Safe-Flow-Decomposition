#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct Graph
{
    int nodes, edges;
    std::string metadata;
    std::vector<std::vector<std::pair<int, double>>> adjacency_list;

    static inline bool store, print;
    static std::ifstream input;

    static void init(char const *argv[])
    {
        Graph::store = strcmp(argv[2], "true") ? false : true;
        Graph::print = strcmp(argv[3], "true") ? false : true;

        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::cout << std::fixed << std::setprecision(0);
        input.close();
        input.open(std::string(argv[1]), std::ifstream::in);
    }

    static std::string read()
    {
        std::string temp;
        while (std::getline(input, temp, '#'))
        {
            if (temp.length() == 0)
                continue;
            return temp;
        }
        return "EOF";
    };

    Graph(const std::string &graph)
    {
        std::stringstream lines(graph);
        std::string temp;

        int line = 0;
        while (std::getline(lines, temp, '\n'))
        {
            if (line == 0)
                metadata = "#" + temp;
            else if (line == 1)
            {
                nodes = stoi(temp);
                adjacency_list.resize(nodes);
            }
            else
            {
                std::stringstream tokens(temp);
                int token = 0;
                int u = -1, v = -1;
                double w = -1;
                while (std::getline(tokens, temp, ' '))
                {
                    if (token == 0)
                        u = stoi(temp);
                    else if (token == 1)
                        v = stoi(temp);
                    else
                        w = stod(temp);
                    token++;
                }
                adjacency_list[u].emplace_back(v, w);
            }
            line++;
        }
        edges = line - 2;
    };

    virtual ~Graph() = default;
    virtual void print_safe_paths() = 0;
};
