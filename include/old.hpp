#pragma once

#include <graph.hpp>
#include <trie.hpp>

typedef std::vector<std::pair<int, double>>::iterator Iterator;
typedef std::tuple<int, int, double> Cut;
template <typename T>
using Path = std::pair<std::deque<int>, std::vector<T>>;

enum ALGO
{
    RAW,
    CONCISE
};

template <ALGO A, bool T = false>
struct Old : public Graph
{
    typedef std::conditional_t<A == RAW, double, std::vector<std::tuple<int, int, double>>> data;

    std::vector<double> f_in, f_out;
    std::vector<std::pair<std::vector<Iterator>, std::pair<int, double>>> st_path;

    std::vector<std::pair<double, std::deque<int>>> raw_repr;
    std::vector<Path<Cut>> concise_repr;

    Old(const std::string &graph);
    ~Old();
    void decompose_path();
    void insert(std::shared_ptr<AC_Trie<data>> &root, std::deque<int> &str);
    void compute_safe();
    void compress_path(data payload, std::deque<int> &route, std::shared_ptr<AC_Trie<data>> &root);

    void print_safe_paths() override;
};
