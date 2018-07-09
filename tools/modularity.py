from igraph import *
import sys

graphname, clustersname = sys.argv[1:]
g = Graph.Read_Edgelist(graphname, directed=False)

membership = [0] * g.vcount()
with open(clustersname, 'r') as f:
    for cid, line in enumerate(f):
        for vid in map(int, line.split()):
            membership[vid] = cid

clusters = VertexClustering(g, membership)
print(g.modularity(clusters))
