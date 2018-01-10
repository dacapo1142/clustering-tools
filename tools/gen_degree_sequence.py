import sys
from collections import defaultdict


pair_record = set()
sequence = defaultdict(int)
for line in sys.stdin:
    vid1, vid2 = map(int, line.split())
    sequence[vid1]+=1
    sequence[vid2]+=1
    
try:
    n = int(sys.argv[1])
    for v in range(n):
        k = sequence[v]
        print('{} {}'.format(v, k))
except:
    for v, k in sequence.items():
        print('{} {}'.format(v, k))        
    