#include "../include/graph.hpp"

Graph::Graph(const std::string& graph) {
    
    std::vector<std::string> line;
    std::stringstream lines(graph);
    std::string temp;
    
    while(std::getline(lines, temp, '\n')) line.push_back(temp);
    
    metadata = line[0];
    nodes = stoi(line[1]);
    edges = line.size()-2;
    adjacency_list.resize(nodes);
    f_max_in.resize(nodes,0);
    f_max_out.resize(nodes,0);
    f_in.resize(nodes,0);
    f_out.resize(nodes,0);
    v_max_in.resize(nodes,-1);
    v_max_out.resize(nodes,-1);
    trie.resize(nodes);
    leaves.resize(nodes);

    for(int i=2;i<edges+2;i++) {

        std::vector<std::string> token;
        std::stringstream tokens(line[i]);        
        while(std::getline(tokens, temp, ' ')) token.push_back(temp);
    
        int u = stoi(token[0]);
        int v = stoi(token[1]);
        double w = stod(token[2]);
        
        adjacency_list[u].push_front({v,w});   
        f_in[v] += w;
        f_out[u] += w;
        if(f_max_in[v] < w) {
            f_max_in[v] = w;
            v_max_in[v] = u;
        }   
        if(f_max_out[u] < w) {
            f_max_out[u] = w;
            v_max_out[u] = v;
        }   
    }

    for(int i=0;i<nodes;i++) {
        if(f_in[i]==0) source=i;
        if(f_out[i]==0) sink=i;
    }

    std::vector<bool> visited(nodes,false);
    for(int i=0;i<nodes;i++) {
        if(!visited[i]) {
            topo_dfs(i, visited);
        }
    }
    std::reverse(topo_order.begin(), topo_order.end());

    for(int i=0;i<nodes;i++) trie[i] = new Trie(i);
}

void Graph::compute_safe(int u) {

    int v_star;
    if((u!=source)&&(u!=sink)) v_star = v_max_out[u];
    else v_star = -1;

    Node* current_node;

    for(std::pair<int,double> p: adjacency_list[u]) {
        int v = p.first;
        if(v==v_star) continue;
        int f_x = p.second;
      

        current_node = trie[v]->insert(u,f_x,trie[v]->head);
        int x = u;

        while((x!=source)&&(f_x-f_in[x]+f_max_in[x]>0)) {
            int k = v_max_in[x];
            current_node = trie[v]->insert(k,f_max_in[x],current_node);
            f_x += f_max_in[x] - f_in[x];
            x = k;
        }
        leaves[v].push_front({current_node,f_x});
    }

    if(v_star!=-1) {
        trie[v_star]->insert(trie[u],f_max_out[u],trie[v_star]->head);
    }
    
    
    for(std::pair<Node*,double> p: leaves[u]) {
        Node* x = p.first;
        int f_x = p.second;
        double excess = f_x-f_out[u]+f_max_out[u];

        if((v_star==-1)||(excess<=0)) {
            
            std::forward_list<int> path;
            while(x->parent_node!=trie[u]->head) {
                path.push_front(x->value);
                x=x->parent_node;
            }
            path.push_front(x->value);
            path.push_front(u);
            path.reverse();
            raw_decomposition.push_front({f_x,path});
        }

        if(v_star!=-1) {
            f_x = excess;
            while((f_x<=0)&&(x->children==0)) {
                Node* y = x->parent_node;
                f_x = f_x+f_in[y->value]-x->flow_to_parent;
                x->parent_node=nullptr;
                x = y;
            }
            if(x->children==0) {
                leaves[v_star].push_front({x,f_x});
            }
        }
    }
    return;
}

void Graph::print_raw_decomposition() {
    for(auto path: raw_decomposition) {
        std::cout<<path.first<<" ";
        for(auto value: path.second) std::cout<<value<<" ";
        std::cout<<std::endl;
    }
    return;
}

void Graph::topo_dfs(int v, std::vector<bool>& visited) {
    visited[v] = true;
    for (std::pair<int,double> u : adjacency_list[v]) {
        if (!visited[u.first]) {
            topo_dfs(u.first, visited);
        }
    }
    topo_order.push_back(v);
    return;
}
