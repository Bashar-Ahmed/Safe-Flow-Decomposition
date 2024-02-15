#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <list>

struct Graph
{
    int nodes, edges;
    std::string metadata;
    std::vector<std::vector<std::pair<int, double>>> adjacency_list;

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

    virtual ~Graph() {}
    virtual void print_maximal_safe_paths() {}

    static std::string read()
    {
        std::string temp;
        while (std::getline(std::cin, temp, '#'))
        {
            if (temp.length() == 0)
                continue;
            return temp;
        }
        return "";
    };
};
