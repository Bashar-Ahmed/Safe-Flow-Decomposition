#include "../include/complete.hpp"
#include "../include/debug.hpp"

Complete::Complete(const std::string& graph) : Graph(graph) {}

void Complete::update(std::list<iterator>& list, double flow) {
	for(auto& edge: list)  {
		edge->second += flow;
	}
    return;
}

void Complete::decompose_path() {

	for(int i=0;i<adjacency_list.size();i++) {
		if(f_in[i]==0) { 
			for(auto edge=adjacency_list[i].begin();edge!=adjacency_list[i].end();edge++) {
				while(edge->second!=0) {

					std::pair<int,double> flow = {i,std::numeric_limits<double>::max()};
					std::list<iterator> route;
					iterator current_edge = edge;

					while(true){

						flow.second = std::min(flow.second, current_edge->second);
						route.push_back(current_edge);

						if(f_out[current_edge->first]==0) break;
						
						auto next_edge=adjacency_list[current_edge->first].begin(); 
						while(next_edge->second==0) next_edge++;
						current_edge = next_edge;
					}
					path.push_back({route,flow});
					update(route, -flow.second);
				}					
			}
		}
	}

	for(auto& edge: path) { 
		update(edge.first,edge.second.second);
	}
	return;
}

void Complete::compute_safe() {
	
	for(auto path_iter=path.begin(); path_iter!=path.end();path_iter++){

		std::list<std::pair<int,double>> route;
		auto left_iter = (*path_iter).first.begin();
		auto right_iter = left_iter;

		int from_node = (*path_iter).second.first;
		int to_node = (**left_iter).first; 
		double flow = (**left_iter).second;

		route.push_back(**left_iter);	
		right_iter++;

		while(true){  
			
			while((right_iter!=(*path_iter).first.end()) && (flow+(**right_iter).second-f_out[to_node]>0)) { 
				flow -= f_out[to_node]-(**right_iter).second;
				to_node = (**right_iter).first;
				route.push_back(**right_iter);
				right_iter++;	
			} 
			
			if(left_iter!=right_iter){  
                result.push_back({route,{from_node,flow}});
			}

			if(right_iter!=(*path_iter).first.end()){
				flow -= f_out[to_node]-(**right_iter).second;
				to_node = (**right_iter).first;
				route.push_back(**right_iter);
				right_iter++;
				
				while((flow-(**left_iter).second+f_in[(**left_iter).first])<=0){
					flow += f_in[(**left_iter).first]-(**left_iter).second; 
					route.pop_front();		
					left_iter++;
				}
		
				from_node = (**left_iter).first;
				flow += f_in[(**left_iter).first]-(**left_iter).second; 
				route.pop_front();		
				left_iter++;
			} 
			else break;
		}
	}
    return;
}

void Complete::insert(AC_Trie root, std::list<int>* str) {
	
	if(root.children.empty()) {
		if(root.is_fail) return;

		std::list<int> path;
		for(auto& y: *str) path.push_back(y);
		complete_repr.push_back({root.flow,path});
	}
	else {
		for(auto& node: root.children){
			str->push_back(node.first);
			insert(node.second,str);
			str->pop_back();
		}
	}
}

void Complete::compress_path() {

	AC_Trie root, *current_node;
	for(auto& path: result){
		if(path.first.size()!=1){

			current_node = &root;	
			std::list<int> nodes;
			int node;
			double flow = path.second.second;

			for(auto path_node: path.first) {
				node = path_node.first;
				nodes.push_back(node);
				auto child = current_node->children.begin();
				for(;child!=current_node->children.end();child++){
					if(child->first == node){
						current_node = &(child->second); 
						break;	
					}
				}
				if(child==current_node->children.end()){
					AC_Trie trie;
					trie.value = node;
					trie.is_fail = false;
					current_node->children.push_back({node,trie});
					current_node = &(current_node->children.back().second);
				}
			}					
			if(nodes.empty()) break;
			current_node->flow=flow;				
		}
	}
	root.add_fail();
	std::list<int> str;
	insert(root, &str);
    return;
}

void Complete::print_maximal_safe_paths() {
	for(auto path: complete_repr) {
        std::cout<<path.first<<" ";
        for(auto value: path.second) std::cout<<value<<" ";
        std::cout<<"\n";
    }
    return;
}
