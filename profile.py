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
    with open("data_1000.json", "r") as file:
        data = json.load(file)

    nodes = data["nodes"]
    edges = data["edges"]

    s2 = numpy.array(data["complete"][1]) / numpy.array(data["raw"][1])
    plt.scatter(nodes, s2, c=colour["raw"], s=0.25)
    # for algo in algos:
    #     for i in range(3):
    #         plt.subplot(2, 3, 1 + i)
    #         plt.xlabel("Nodes")
    #         plt.ylabel(labels[i])
    #         plt.scatter(nodes, data[algo][i], c=colour[algo], s=0.25)
    #         plt.subplot(2, 3, 4 + i)
    #         plt.xlabel("Edges")
    #         plt.ylabel(labels[i])
    #         plt.scatter(edges, data[algo][i], c=colour[algo], s=0.25)
    #         plt.scatter(edges, m, c="green", s=0.25)

    plt.tight_layout()
    plt.savefig("plot_1000_ex.png")


plot_graph()
