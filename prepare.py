import os
import subprocess

def is_funnel(graph):
    lines = [line for line in graph.split('\n') if len(line)>0]
    n = int(lines[1])
    adj = [[] for i in range(n)]
    radj = [[] for i in range(n)]
    lines = lines[2:]
    for g in lines:
        v = g.split(' ')
        a = int(v[0])
        b = int(v[1])
        adj[a].append(b)
        radj[b].append(a)

    def left(i):
        if len(radj[i])>1:
            return False
        if len(radj[i])==0:
            return True
        return left(radj[i][0])
    def right(i):
        if len(adj[i])>1:
            return False
        if len(adj[i])==0:
            return True
        return right(adj[i][0])
    for i in range(n):
        if left(i):
            continue
        if right(i):
            continue
        return False
    return True

dataset = "catfish"

files = subprocess.getoutput(f"find ./data/{dataset} -type f -wholename '*.graph'")
files = [file_name for file_name in files.split("\n") if len(file_name) > 0]

for file_name in files:
    print(file_name)
    with open(file_name, "r") as f:
        graphs = ["#" + graph for graph in f.read().split("#") if len(graph) > 0]
        trivial_file_name = file_name.replace("data", "data/preprocessed/multiple/trivial")
        non_trivial_file_name = file_name.replace("data", "data/preprocessed/multiple/nontrivial")
        os.makedirs(os.path.dirname(trivial_file_name), exist_ok=True)
        os.makedirs(os.path.dirname(non_trivial_file_name), exist_ok=True)
        trivial=""
        non_trivial=""

        file_name = file_name.replace(".graph", "")
        trivial_folder_name = file_name.replace("data", "data/preprocessed/single/trivial")
        non_trivial_folder_name = file_name.replace("data", "data/preprocessed/single/nontrivial")
        os.makedirs(trivial_folder_name, exist_ok=True)
        os.makedirs(non_trivial_folder_name, exist_ok=True)

        for i, graph in enumerate(graphs):
            trivial_graph_name = trivial_folder_name + f"/{i}.graph"
            non_trivial_graph_name = non_trivial_folder_name + f"/{i}.graph"

            if is_funnel(graph):
                trivial+=graph
                with open(trivial_graph_name,"w") as g:
                    g.write(graph)
                continue

            non_trivial+=graph
            with open(non_trivial_graph_name,"w") as g:
                g.write(graph)

        with open(trivial_file_name,"w") as g:
            g.write(trivial)
        with open(non_trivial_file_name,"w") as g:
            g.write(non_trivial)               
