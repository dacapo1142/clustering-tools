import fileinput
import sys

listset, groundtruth=sys.argv[1:3]
with open(listset) as f:
    s = {line.strip() for line in f}

with open(groundtruth) as f:
    for line in f:
        newgroup = list(filter(lambda x: x in s, line.strip().split()))
        if newgroup:
            print(' '.join(newgroup))
