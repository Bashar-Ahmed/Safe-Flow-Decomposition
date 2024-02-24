#pragma once

#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

inline std::string zebrafish_graph = "../test/graph.txt";
inline std::string zebrafish_truth = "../test/truth.txt";
inline std::string mouse_graph = "../test/graph.txt";
inline std::string mouse_truth = "../test/truth.txt";
inline std::string human_graph = "../test/graph.txt";
inline std::string human_truth = "../test/truth.txt";
inline std::string salmon_graph = "../test/graph.txt";
inline std::string salmon_truth = "../test/truth.txt";
inline std::string refsim_graph = "../test/graph.txt";
inline std::string refsim_truth = "../test/truth.txt";
inline std::string simulation_graph = "../test/graph.txt";
inline std::string simulation_truth = "../test/truth.txt";

#define RAWREP                                \
    auto *graph = new Old<RAW>(graph_string); \
    graph->decompose_path();                  \
    graph->compute_safe();

#define RAWREPT                                     \
    auto *graph = new Old<RAW, true>(graph_string); \
    graph->decompose_path();                        \
    graph->compute_safe();

#define CONREP                                    \
    auto *graph = new Old<CONCISE>(graph_string); \
    graph->decompose_path();                      \
    graph->compute_safe();

#define CONREPT                                         \
    auto *graph = new Old<CONCISE, true>(graph_string); \
    graph->decompose_path();                            \
    graph->compute_safe();

#define OPTRAW                           \
    auto *graph = new Raw(graph_string); \
    for (int u : graph->topo_order)      \
        graph->compute_safe(u);

#define OPTCON                               \
    auto *graph = new Concise(graph_string); \
    for (int u : graph->topo_order)          \
        graph->compute_safe(u);

#define OPTCONH                                    \
    auto *graph = new Concise<true>(graph_string); \
    for (int u : graph->topo_order)                \
        graph->compute_safe(u);

#define OPTREP                               \
    auto *graph = new Optimal(graph_string); \
    graph->compute_non_trivial();            \
    graph->compute_trivial();

#define OPTREPH                                    \
    auto *graph = new Optimal<true>(graph_string); \
    graph->compute_non_trivial();                  \
    graph->compute_trivial();
