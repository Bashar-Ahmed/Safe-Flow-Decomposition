#include "../include/io.hpp"

std::string read_graph()
{
    std::string temp;
    while (std::getline(std::cin, temp, '#'))
    {
        if (temp.length() == 0)
            continue;
        return temp;
    }
    return "";
}
