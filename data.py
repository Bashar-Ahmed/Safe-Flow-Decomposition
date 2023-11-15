import json
import subprocess

algos = ["complete", "raw", "concise", "optimal"]
dataset = "single/nontrivial/catfish/simulation"

graphs = subprocess.getoutput(f"find ../data/preprocessed/{dataset} -type f -wholename '*.graph'")
graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

complete = [[], [], []]
raw = [[], [], []]
concise = [[], [], []]
optimal = [[], [], []]

nodes = []
edges = []

for graph in graphs:
    
    with open(graph, "r") as file:
        lines = [line for line in file.read().split('\n') if len(line)>0]
        nodes.append(int(lines[1]))
        edges.append(len(lines)-2)

    print(graph)

    for algo in algos:

        data = subprocess.getoutput(f"/usr/bin/time -v ./build/{algo} < {graph} > temp.txt")
        stats = [stat for stat in data.replace(" ", "").split("\n")]
        time = float(stats[1].split(":")[1]) + float(stats[2].split(":")[1])
        memory = int(stats[9].split(":")[1])
        tokens = 0

        with open("temp.txt","r") as file:
            lines = file.read().split("\n")
            lines = [line.split(" ") for line in lines if len(line)>0 and line[0]!='#']
            words = [j for sub in lines for j in sub if len(j)>0]
            tokens += len(words)

        globals()[algo][0].append(tokens/100)
        globals()[algo][1].append(round(time, 2))
        globals()[algo][2].append(memory)

data = {"nodes": nodes, "edges": edges, "complete": complete, "raw": raw, "concise": concise, "optimal":optimal}

with open("data.json", "w") as file:
    json.dump(data, file)
