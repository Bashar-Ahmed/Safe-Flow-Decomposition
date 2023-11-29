#include "../include/complete.hpp"

Complete::Complete(const std::string &graph) : Graph(graph)
{
	f_in.resize(nodes, 0);
	f_out.resize(nodes, 0);

	for (int i = 0; i < nodes; i++)
	{
		for (auto &&edge : adjacency_list[i])
		{

			int u = i;
			int v = edge.first;
			double w = edge.second;

			f_in[v] += w;
			f_out[u] += w;
		}
	}
}

void Complete::decompose_path()
{

	for (int i = 0; i < nodes; i++)
	{
		if (f_in[i] == 0)
		{
			for (auto edge = adjacency_list[i].begin(); edge != adjacency_list[i].end(); edge++)
			{
				while (edge->second != 0)
				{

					std::pair<int, double> flow = {i, std::numeric_limits<double>::max()};
					std::vector<iterator> route;
					iterator current_edge = edge;

					while (true)
					{

						route.emplace_back(current_edge);
						auto current_edge_value = *current_edge;
						flow.second = std::min(flow.second, current_edge_value.second);

						if (f_out[current_edge_value.first] == 0)
							break;

						auto next_edge = adjacency_list[current_edge_value.first].begin();
						while (next_edge->second == 0)
							next_edge++;
						current_edge = next_edge;
					}

					for (auto &&edge : route)
						edge->second -= flow.second;
					path.emplace_back(std::move(route), flow);
				}
			}
		}
	}

	for (auto &&edge_list : path)
		for (auto &&edge : edge_list.first)
			edge->second += edge_list.second.second;
	return;
}

void Complete::insert(std::shared_ptr<AC_Trie> &root, std::vector<int> &str)
{

	if (root->children.empty())
	{
		if (root->is_fail)
			return;
		complete_repr.emplace_back(root->flow, str);
	}
	else
	{
		for (auto &&node : root->children)
		{
			str.emplace_back(node.first);
			insert(node.second, str);
			str.pop_back();
		}
	}
}

void Complete::compute_safe()
{

	std::shared_ptr<AC_Trie> root = std::make_shared<AC_Trie>();
	for (auto &&path_value : path)
	{

		std::deque<int> route;
		auto left_iter = path_value.first.begin();
		auto right_iter = left_iter;
		auto left_value = **left_iter;
		auto right_value = **right_iter;

		int from_node = path_value.second.first;
		int to_node = left_value.first;
		double flow = left_value.second;

		route.emplace_back(from_node);
		route.emplace_back(to_node);
		right_iter++;
		right_value = **right_iter;

		while (true)
		{
			auto path_end = path_value.first.end();
			if (right_iter != path_end)
			{
				while (flow + right_value.second - f_out[to_node] > 0)
				{
					flow -= f_out[to_node] - right_value.second;
					to_node = right_value.first;
					route.emplace_back(to_node);
					right_iter++;
					if (right_iter != path_end)
						right_value = **right_iter;
					else
						break;
				}
			}

			if ((left_iter != right_iter) && (route.size() > 2))
				compress_path(flow, route, root);

			if (right_iter != path_end)
			{
				flow -= f_out[to_node] - right_value.second;
				to_node = right_value.first;
				route.emplace_back(to_node);
				right_iter++;
				if (right_iter != path_end)
					right_value = **right_iter;

				while ((flow - left_value.second + f_in[left_value.first]) <= 0)
				{
					flow += f_in[left_value.first] - left_value.second;
					route.pop_front();
					left_iter++;
					if (left_iter != path_end)
						left_value = **left_iter;
				}

				from_node = left_value.first;
				flow += f_in[left_value.first] - left_value.second;
				route.pop_front();
				left_iter++;
				if (left_iter != path_end)
					left_value = **left_iter;
			}
			else
				break;
		}
	}
	root->add_fail();
	std::vector<int> str;
	insert(root, str);
	return;
}

void Complete::compress_path(double flow, std::deque<int> &route, std::shared_ptr<AC_Trie> &root)
{

	auto current_node = root;
	for (auto &&path_node : route)
	{
		auto &children = current_node->children;
		int children_count = children.size();
		bool list_end = true;
		for (int i = 0; i < children_count; i++)
		{
			if (children[i].first == path_node)
			{
				current_node = children[i].second;
				list_end = false;
				break;
			}
		}
		if (list_end)
		{
			std::shared_ptr<AC_Trie> trie = std::make_shared<AC_Trie>();
			trie->value = path_node;
			trie->is_fail = false;
			current_node->children.emplace_back(path_node, std::move(trie));
			current_node = current_node->children.back().second;
		}
	}
	current_node->flow = flow;
	return;
}

void Complete::print_maximal_safe_paths()
{
	std::cout << metadata << "\n\n";
	for (auto &&path : complete_repr)
	{
		std::cout << path.first << " ";
		for (auto &&value : path.second)
			std::cout << value << " ";
		std::cout << "\n";
	}
	return;
}
