#include "../include/io.hpp"

std::pair<std::string,bool> read_graph() {
    static std::string temp = "";
    std::string graph = temp+"\n";
    while(std::getline(std::cin,temp)) {
        if(temp[0]=='#') return {graph, true};
        else graph+=temp+"\n";
    }
    return {graph, false};
}
