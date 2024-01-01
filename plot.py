import numpy
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_excel('data_1000.xlsx')

def plot_graph():

    algos = ["COMPLETE", "RAW", "CONCISE", "OPTIMAL"]
    colour = {"RAW": "blue", "CONCISE": "yellow", "OPTIMAL": "green", "COMPLETE": "red", }

    plt.figure(dpi=1200)

    nodes = df["NODES"].to_numpy()
    edges = df["EDGES"].to_numpy()

    for label in ["TIME", "MEMORY", "TOKENS"]:
        plt.xlabel("NODES")
        plt.ylabel(label)
        for algo in algos:
            plt.scatter(nodes, df[f'{algo} {label}'], c=colour[algo], s=0.25)
        plt.savefig(f"{label}xNODES.png")
        plt.clf()
        plt.xlabel("EDGES")
        plt.ylabel(label)
        for algo in algos:
            plt.scatter(edges, df[f'{algo} {label}'], c=colour[algo], s=0.25)
        plt.savefig(f"{label}xEDGES.png")
        plt.clf()

plot_graph()
