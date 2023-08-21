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

void Complete::compress_path() {
    // 
    return;
}

void Complete::print_complete_decomposition() {

    for(auto it=result.begin();it!=result.end();it++){
		if((*it).first.size()!=1){
			std::cout<<(*it).second.second<<" "<<(*it).second.first;
			for(auto it2=(*it).first.begin();it2!=(*it).first.end();it2++)
				std::cout<<" "<<(*it2).first;
			std::cout<<"\n";
		}
	}
	
    return;
}
