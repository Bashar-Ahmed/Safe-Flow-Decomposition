#include "../include/graph.hpp"

Graph::Graph(const std::string& graph) {
    
    std::vector<std::string> line;
    std::stringstream lines(graph);
    std::string temp;
    
    while(std::getline(lines, temp, '\n')) line.push_back(temp);
    
    metadata = line[0];
    nodes = stoi(line[1]);
    edges = line.size()-2;
    f_in.resize(nodes,0);
    f_out.resize(nodes,0);
    adjacency_list.resize(nodes);

    for(int i=2;i<edges+2;i++) {

        std::vector<std::string> token;
        std::stringstream tokens(line[i]);        
        while(std::getline(tokens, temp, ' ')) token.push_back(temp);
    
        int u = stoi(token[0]);
        int v = stoi(token[1]);
        double w = stod(token[2]);
        
        adjacency_list[u].push_back({v,w}); 

        f_in[v] += w;
        f_out[u] += w;
    }
}
