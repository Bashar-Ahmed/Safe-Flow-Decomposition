import json
import subprocess

graphs = subprocess.getoutput(
    "find . -type f -wholename './data/simulation/param2/1000.100.100.graph'"
)
graphs = [graph for graph in graphs.split("\n") if len(graph) > 0]

algo = "raw"
time = 0.0
memory = 0
fault = 0
for graph in graphs:
    print(graph)
    # for i in range(10):
    data = subprocess.getoutput(
        f"bash time.sh /usr/bin/time -v ./build/{algo} profile < {graph}"
    )
    stats = [stat for stat in data.replace(" ", "").split("\n")]
    memory += int(stats[12].split(":")[1])
    time += float(stats[-1].split(":")[1])
    fault += int(stats[15].split(":")[1])

# print(fault)
print(time)
