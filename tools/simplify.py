from igraph import *
import sys
g = Graph.Read_Edgelist(sys.argv[1], directed=False)
g.write_edgelist(sys.argv[2])
