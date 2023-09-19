import os
import subprocess

files = subprocess.getoutput("find . -type f -wholename './data/simulation/*.graph'")
files = [file_name for file_name in files.split("\n") if len(file_name) > 0]

for file_name in files:
    print(file_name)
    with open(file_name, "r") as f:
        graphs = ["#" + graph for graph in f.read().split("#") if len(graph) > 0]
        file_name = file_name.replace(".graph", "")
        new_folder_name = file_name.replace("data", "data/final")
        os.makedirs(new_folder_name, exist_ok=True)

        for i, graph in enumerate(graphs):
            new_file_name = new_folder_name + f"/{i}.graph"
            with open(new_file_name, "w") as g:
                g.write(graph)
