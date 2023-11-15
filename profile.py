import json
import numpy
import matplotlib.pyplot as plt


def plot_graph():

    algos = ["complete", "raw", "concise", "optimal"]
    labels = ["Tokens", "Time", "Memory"]
    colour = {"complete": "red", "raw": "blue", "concise": "yellow", "optimal": "green"}

    plt.figure(dpi=1200)

    data = {}
    with open("data.json", "r") as file:
        data = json.load(file)

    nodes = data["nodes"]
    edges = data["edges"]

    for i in range(3):
        plt.xlabel("Nodes")
        plt.ylabel(labels[i])
        for algo in algos:
            plt.scatter(nodes, data[algo][i], c=colour[algo], s=0.25)
        plt.savefig(f"report_{1+i}.png")
        plt.clf()
        plt.xlabel("Edges")
        plt.ylabel(labels[i])
        for algo in algos:
            plt.scatter(edges, data[algo][i], c=colour[algo], s=0.25)
        plt.savefig(f"report_{4+i}.png")
        plt.clf()

plot_graph()
