import json
import subprocess

algos = ["complete", "raw"]

graphs = subprocess.getoutput("find . -type f -wholename './data/simulation/*.graph'")
graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

complete = [[], [], []]
raw = [[], [], []]
nodes = []
edges = []

for algo in algos:
    nodes = []
    edges = []
    for graph in graphs:
        print(graph)
        data = subprocess.getoutput(
            f"bash time.sh /usr/bin/time -v ./build/{algo} profile < {graph}"
        )
        stats = [stat for stat in data.replace(" ", "").split("\n")]
        node = int(stats[0].split(":")[1])
        edge = int(stats[1].split(":")[1])
        tokens = int(stats[2].split(":")[1])
        # time = float(stats[4].split(":")[1]) + float(stats[5].split(":")[1])
        memory = int(stats[12].split(":")[1])
        time = float(stats[-1].split(":")[1])
        nodes.append(node)
        edges.append(edge)
        globals()[algo][0].append(tokens)
        globals()[algo][1].append(round(time, 9))
        globals()[algo][2].append(memory)

# print(raw)
# print(complete)

data = {"nodes": nodes, "edges": edges, "raw": raw, "complete": complete}

with open("data_O3.json", "w") as file:
    json.dump(data, file)
