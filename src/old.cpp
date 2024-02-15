#include "../include/old.hpp"

Old::Old(const std::string &graph, bool concise, bool ac_trie) : Graph(graph), concise(concise), ac_trie(ac_trie)
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

void Old::decompose_path()
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
					st_path.emplace_back(std::move(route), flow);
				}
			}
		}
	}

	for (auto &&edge_list : st_path)
		for (auto &&edge : edge_list.first)
			edge->second += edge_list.second.second;
	return;
}

template <typename T>
void Old::insert(std::shared_ptr<AC_Trie<T>> &root, std::vector<int> &str)
{

	if (root->children.empty())
	{
		if (root->is_fail)
			return;
		raw_repr.emplace_back(root->data, str);
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

void Old::compute_safe()
{
	std::shared_ptr<AC_Trie<double>> root = std::make_shared<AC_Trie<double>>();
	std::shared_ptr<AC_Trie<std::vector<cut>>> root_concise = std::make_shared<AC_Trie<std::vector<cut>>>();

	for (auto &&path_value : st_path)
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
			{
				if (concise)
				{
					if (ac_trie)
					{
					}
					else
					{
						if (concise_repr.empty())
							concise_repr.emplace_back(route, std::vector<cut>({cut(route.front(), route.back(), flow)}));
						else
						{
							auto &end = concise_repr.back();
							auto it = route.begin();
							bool subpath = false;
							for (auto &&path_element : end.first)
							{
								if (it == route.end())
								{
									subpath = true;
									break;
								}
								if (*it == path_element)
									it++;
								else
									it = route.begin();
							}
							if (subpath || it == route.begin())
								concise_repr.emplace_back(route, std::vector<cut>({cut(route.front(), route.back(), flow)}));
							else
							{
								while (it != route.end())
									end.first.emplace_back(*it++);
								end.second.emplace_back(cut(route.front(), route.back(), flow));
							}
						}
					}
				}
				else
				{
					if (ac_trie)
						compress_path(flow, route, root);
					else
						raw_repr_wo_trie.emplace_back(flow, route);
				}
			}

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

template <typename T>
void Old::compress_path(double flow, std::deque<int> &route, std::shared_ptr<AC_Trie<T>> &root)
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
			std::shared_ptr<AC_Trie<T>> trie = std::make_shared<AC_Trie<T>>();
			trie->value = path_node;
			trie->is_fail = false;
			current_node->children.emplace_back(path_node, std::move(trie));
			current_node = current_node->children.back().second;
		}
	}
	if (!concise)
		current_node->data = flow;
	return;
}

void Old::print_maximal_safe_paths()
{
	std::cout << metadata << "\n\n";
	for (auto &&path : raw_repr)
	{
		std::cout << path.first << " ";
		for (auto &&value : path.second)
			std::cout << value << " ";
		std::cout << "\n";
	}
	return;
}
