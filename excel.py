import subprocess
import pandas as pd

# file = 'data.xlsx'
file = 'data_1000.xlsx'

df = pd.read_excel(file)

datasets = [
            # "multiple/nontrivial/catfish/rnaseq/human",
            # "multiple/nontrivial/catfish/rnaseq/mouse",
            # "multiple/nontrivial/catfish/rnaseq/salmon",
            # "multiple/nontrivial/catfish/rnaseq/zebrafish",
            # "multiple/nontrivial/catfish/simulation",
            # "multiple/trivial/catfish/rnaseq/human",
            # "multiple/trivial/catfish/rnaseq/mouse",
            # "multiple/trivial/catfish/rnaseq/salmon",
            # "multiple/trivial/catfish/rnaseq/zebrafish",
            # "multiple/trivial/catfish/simulation",
            "single/nontrivial/catfish/simulation",
            ]

for dataset in datasets:

    graphs = subprocess.getoutput(f"find ../data/preprocessed/{dataset} -type f -wholename '*.graph'")
    graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

    col = ['GRAPH','PATH','MULTIPLE/SINGLE','TRIVIALITY','DATASET',]
    
    for graph in graphs:
        print(graph)
        row = [graph, dataset]
        row.extend(dataset.split('/')[:3])
        df=pd.concat([df, pd.DataFrame([row], columns=col)])

df = df[df.filter(regex='^(?!Unnamed)').columns]
df.to_excel(file, index=False)
                