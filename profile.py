import os
import subprocess
import matplotlib.pyplot as plt

algos = ["complete", "raw"]

graphs = subprocess.getoutput("find . -type f -wholename './data/*.graph'")
graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

complete = [[], [], []]
raw = [[], [], []]
colour = {"complete": "blue", "raw": "red", "concise": "yellow", "optimal": "xyz"}
labels = ["Tokens", "Time", "Memory"]

plt.figure(dpi=1200)

for algo in algos:
    nodes = []
    edges = []
    for graph in graphs:
        print(graph)
        if "1" in graph:
            continue
        data = subprocess.getoutput(
            f"/usr/bin/time -v ./build/{algo} profile < {graph}"
        )
        stats = [stat for stat in data.replace(" ", "").split("\n")]
        node = int(stats[0].split(":")[1])
        edge = int(stats[1].split(":")[1])
        tokens = int(stats[2].split(":")[1])
        time = float(stats[4].split(":")[1]) + float(stats[5].split(":")[1])
        memory = int(stats[12].split(":")[1])
        nodes.append(node)
        edges.append(edge)
        globals()[algo][0].append(tokens)
        globals()[algo][1].append(round(time, 2))
        globals()[algo][2].append(memory)

    for i in range(3):
        plt.subplot(2, 3, 1 + i)
        plt.xlabel("Nodes")
        plt.ylabel(labels[i])
        plt.scatter(nodes, globals()[algo][i], c=colour[algo], s=3)
        plt.subplot(2, 3, 4 + i)
        plt.xlabel("Edges")
        plt.ylabel(labels[i])
        plt.scatter(edges, globals()[algo][i], c=colour[algo], s=3)
print(raw)
print(complete)
plt.tight_layout()
plt.savefig("plot2.png")
