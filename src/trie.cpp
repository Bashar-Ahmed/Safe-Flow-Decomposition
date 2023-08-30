#include "../include/trie.hpp"

Node::Node(int u)
{
	value = u;
	children = 0;
	flow_to_parent = 0.0;
	parent_node = nullptr;
}

Node::Node(int u, double flow, std::shared_ptr<Node> parent)
{
	value = u;
	children = 0;
	flow_to_parent = flow;
	parent_node = parent;
}

Trie::Trie(int u)
{
	head = std::make_shared<Node>(u);
}

std::shared_ptr<Node> Trie::insert(int u, double flow, std::shared_ptr<Node> parent)
{
	std::shared_ptr<Node> node = std::make_shared<Node>(u, flow, parent);
	parent->children++;
	return node;
}

void Trie::insert(std::unique_ptr<Trie> &u, double flow, std::shared_ptr<Node> parent)
{
	u->head->parent_node = parent;
	u->head->flow_to_parent = flow;
	parent->children++;
	return;
}

void AC_Trie::add_fail()
{
	std::shared_ptr<AC_Trie> shared_this = shared_from_this();
	this->fail = shared_this;
	std::queue<std::shared_ptr<AC_Trie>> q;
	q.push(shared_this);

	while (!q.empty())
	{
		auto source = q.front();
		q.pop();
		for (auto &source_node : source->children)
		{
			auto target = source;
			source_node.second->fail.reset();
			while (target.get() != this)
			{
				target = target->fail.lock();
				for (auto &target_node : target->children)
				{
					if (source_node.first == target_node.first)
					{
						source_node.second->fail = target_node.second;
						target_node.second->is_fail = true;
						target = shared_this;
						break;
					}
				}
			}
			if (source_node.second->fail.lock().get() == nullptr)
				source_node.second->fail = shared_this;
			q.push(source_node.second);
		}
	}
}
