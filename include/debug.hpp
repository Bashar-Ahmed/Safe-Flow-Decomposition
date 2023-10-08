#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>

template <typename Node>
void __p(std::shared_ptr<Node> a);
template <typename Node>
void __p(std::weak_ptr<Node> a);
template <typename T>
void __p(T a);
template <typename T, typename F>
void __p(std::pair<T, F> a);
template <typename T, typename F, typename G>
void __p(std::tuple<T, F, G> a);
template <typename T>
void __p(std::list<T> a);
template <typename T>
void __p(std::vector<T> a);
template <typename T>
void __p(std::set<T> a);
template <typename T>
void __p(std::multiset<T> a);
template <typename T, typename F>
void __p(std::map<T, F> a);
template <typename T>
void __p(std::deque<T> a);
template <typename T>
void __p(std::queue<T> a);
template <typename T>
void __p(std::stack<T> a);

template <typename Node>
void __p(std::shared_ptr<Node> a)
{
    std::cout << "{";
    __p(a->value);
    std::cout << ",";
    __p(a.get());
    std::cout << "}";
}

// template <typename Node>
// void __p(std::weak_ptr<Node> a)
// {
//     __p(a);
// }

template <typename T>
void __p(T a)
{
    std::cout << a;
}

template <typename T, typename F>
void __p(std::pair<T, F> a)
{
    std::cout << "{";
    __p(a.first);
    std::cout << ",";
    __p(a.second);
    std::cout << "}";
}

template <typename T, typename F, typename G>
void __p(std::tuple<T, F, G> a)
{
    std::cout << "{";
    __p(std::get<0>(a));
    std::cout << ",";
    __p(std::get<1>(a));
    std::cout << ",";
    __p(std::get<2>(a));
    std::cout << "}";
}

template <typename T>
void __p(std::list<T> a)
{
    std::cout << "{";
    for (auto it = a.begin(); it != a.end();)
    {
        __p(*it);
        std::cout << ",}"[++it == a.end()];
    }
}
template <typename T>
void __p(std::vector<T> a)
{
    std::cout << "{";
    for (auto it = a.begin(); it < a.end(); it++)
        __p(*it), std::cout << ",}"[it + 1 == a.end()];
}

template <typename T>
void __p(std::set<T> a)
{
    std::cout << "{";
    for (auto it = a.begin(); it != a.end();)
    {
        __p(*it);
        std::cout << ",}"[++it == a.end()];
    }
}

template <typename T>
void __p(std::deque<T> a)
{
    std::cout << "{";
    while (!a.empty())
    {
        __p(a.front());
        a.pop_front();
        std::cout << ",}"[a.empty()];
    }
}

template <typename T>
void __p(std::queue<T> a)
{
    std::cout << "{";
    while (!a.empty())
    {
        __p(a.front());
        a.pop();
        std::cout << ",}"[a.empty()];
    }
}

template <typename T>
void __p(std::stack<T> a)
{
    std::cout << "{";
    while (!a.empty())
    {
        __p(a.top());
        a.pop();
        std::cout << ",}"[a.empty()];
    }
}

template <typename T>
void __p(std::multiset<T> a)
{
    std::cout << "{";
    for (auto it = a.begin(); it != a.end();)
    {
        __p(*it);
        std::cout << ",}"[++it == a.end()];
    }
}

template <typename T, typename F>
void __p(std::map<T, F> a)
{
    std::cout << "{\n";
    for (auto it = a.begin(); it != a.end(); ++it)
    {
        __p(it->first);
        std::cout << ": ";
        __p(it->second);
        std::cout << "\n";
    }
    std::cout << "}\n";
}

template <typename T, typename... Arg>
void __p(T a1, Arg... a)
{
    __p(a1);
    __p(a...);
}

template <typename Arg1>
void __f(const char *name, Arg1 &&arg1)
{
    std::cout << name << " : ";
    __p(arg1);
    std::cout << std::endl;
}

template <typename Arg1, typename... Args>
void __f(const char *names, Arg1 &&arg1, Args &&...args)
{
    int bracket = 0, i = 0;
    for (;; i++)
        if (names[i] == ',' && bracket == 0)
            break;
        else if (names[i] == '(')
            bracket++;
        else if (names[i] == ')')
            bracket--;
    const char *comma = names + i;
    std::cout.write(names, comma - names) << " : ";
    __p(arg1);
    std::cout << " | ";
    __f(comma + 1, args...);
}

#define trace(...) std::cout << "Line:" << __LINE__ << " ", __f(#__VA_ARGS__, __VA_ARGS__)
