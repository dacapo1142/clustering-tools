from sklearn.metrics.cluster import normalized_mutual_info_score
import sys
from collections import deque

file1, file2 = sys.argv[1:3]

v1 = deque()
v2 = deque()
d = dict()
reindex = 0
with open(file1) as f:
    for cid, line in enumerate(f):
        vlabels = line.strip().split()
        v1.extend([cid] * len(vlabels))
        for vlabel in vlabels:
            d[vlabel] = reindex
            reindex += 1
v1 = list(v1)
v2 = [0] * len(v1)
with open(file2) as f:
    for cid, line in enumerate(f):
        vlabels = line.strip().split()
        for vlabel in vlabels:
            v2[d[vlabel]] = cid

print(normalized_mutual_info_score(v1, v2))