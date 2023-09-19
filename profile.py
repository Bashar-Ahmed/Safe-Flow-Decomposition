import json
import numpy
import matplotlib.pyplot as plt


def plot_graph():
    # TODO:
    algos = ["complete", "raw"]
    labels = ["Tokens", "Time", "Memory"]
    colour = {"complete": "red", "raw": "blue", "concise": "yellow", "optimal": "green"}

    plt.figure(dpi=1200)

    data = {}
    with open("data_O3.json", "r") as file:
        data = json.load(file)

    nodes = data["nodes"]
    edges = data["edges"]

    for algo in algos:
        for i in range(3):
            plt.subplot(2, 3, 1 + i)
            plt.xlabel("Nodes")
            plt.ylabel(labels[i])
            plt.scatter(nodes, data[algo][i], c=colour[algo], s=0.25)
            plt.subplot(2, 3, 4 + i)
            plt.xlabel("Edges")
            plt.ylabel(labels[i])
            plt.scatter(edges, data[algo][i], c=colour[algo], s=0.25)

    plt.tight_layout()
    plt.savefig("plotxyzO3.png")


plot_graph()
