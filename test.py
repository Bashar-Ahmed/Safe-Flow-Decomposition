import json
import subprocess

data_file = open("data.txt", "w")

datasets = [
            # "multiple/nontrivial/catfish/rnaseq/human",
            # "multiple/nontrivial/catfish/rnaseq/mouse",
            # "multiple/nontrivial/catfish/rnaseq/salmon",
            # "multiple/nontrivial/catfish/rnaseq/zebrafish",
            "multiple/nontrivial/catfish/simulation",
            # "multiple/trivial/catfish/rnaseq/human",
            # "multiple/trivial/catfish/rnaseq/mouse",
            # "multiple/trivial/catfish/rnaseq/salmon",
            # "multiple/trivial/catfish/rnaseq/zebrafish",
            # "multiple/trivial/catfish/simulation",
            ]

algos = [
        "complete",
        # "raw", 
        # "concise",
        # "optimal"
        ]

for dataset in datasets:

    graphs = subprocess.getoutput(f"find ../data/preprocessed/{dataset} -type f -wholename '*1000.100.100.graph'")
    graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

    data_file.write(dataset+"\n")
    for algo in algos:
        time = 0.0
        memory = 0
        token = 0 
        for i in range(1):
            for graph in graphs:
                print(graph)
                data = subprocess.getoutput(f"/usr/bin/time -v ./build/{algo} < {graph} > temp.txt")
                stats = [stat for stat in data.replace(" ", "").split("\n")]
                for stat in stats:
                    print(stat)
                time += float(stats[1].split(":")[1])
                time += float(stats[2].split(":")[1])
                memory = max(memory,int(stats[9].split(":")[1]))
                with open("temp.txt","r") as file:
                    lines = file.read().split("\n")
                    lines = [line.split(" ") for line in lines if len(line)>0 and line[0]!='#']
                    words = [j for sub in lines for j in sub if len(j)>0]
                    token+=len(words)

        data_file.write(algo)
        data_file.write(f"\nTOKEN: {token}    ")
        data_file.write(f"TIME: {round(time,2)}  ")
        data_file.write(f"MEMORY: {memory}\n")
        data_file.flush()
