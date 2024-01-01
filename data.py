import subprocess
import pandas as pd

df = pd.read_excel('data.xlsx')

datasets = [
            "multiple/nontrivial/catfish/rnaseq/human",
            "multiple/nontrivial/catfish/rnaseq/mouse",
            "multiple/nontrivial/catfish/rnaseq/salmon",
            "multiple/nontrivial/catfish/rnaseq/zebrafish",
            "multiple/nontrivial/catfish/simulation",
            "multiple/trivial/catfish/rnaseq/human",
            "multiple/trivial/catfish/rnaseq/mouse",
            "multiple/trivial/catfish/rnaseq/salmon",
            "multiple/trivial/catfish/rnaseq/zebrafish",
            "multiple/trivial/catfish/simulation",
            ]

algos = [
        "raw", 
        "concise",
        "optimal",
        "complete",
        ]

# df["RAW TIME"] = -1
# df["RAW MEMORY"] = -1
# df["RAW TOKENS"] = -1
# df["CONCISE TIME"] = -1
# df["CONCISE MEMORY"] = -1
# df["CONCISE TOKENS"] = -1
# df["OPTIMAL TIME"] = -1
# df["OPTIMAL MEMORY"] = -1
# df["OPTIMAL TOKENS"] = -1
# df["COMPLETE TIME"] = -1
# df["COMPLETE MEMORY"] = -1
# df["COMPLETE TOKENS"] = -1

for dataset in datasets:

    graphs = subprocess.getoutput(f"find ../data/preprocessed/{dataset} -type f -wholename '*.graph'")
    graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

    for algo in algos:
        for i in range(1):
            for graph in graphs:
                print(graph)
                output = graph.replace("graph","truth")
                data = subprocess.getoutput(f"/usr/bin/time -v ./build/{algo} < {graph} > {output}")
                stats = [stat for stat in data.replace(" ", "").split("\n")]
                time = round(float(stats[1].split(":")[1]) + float(stats[2].split(":")[1]),2)
                memory = int(stats[9].split(":")[1])
                token = 0
                
                with open(output,"r") as file:
                    lines = file.read().split("\n")
                    lines = [line.split(" ") for line in lines if len(line)>0 and line[0]!='#']
                    words = [j for sub in lines for j in sub if len(j)>0]
                    token = len(words)
                
                df.loc[df.GRAPH == graph, f'{algo.upper()} TIME'] = time
                df.loc[df.GRAPH == graph, f'{algo.upper()} MEMORY'] = memory
                df.loc[df.GRAPH == graph, f'{algo.upper()} TOKENS'] = token

df = df[df.filter(regex='^(?!Unnamed)').columns]
df.to_excel('data.xlsx', index=False)

# df[df.PATH=="multiple/nontrivial/catfish/rnaseq/salmon"]["OPTIMAL TIME"].sum()
