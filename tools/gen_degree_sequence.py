import sys
from collections import defaultdict

pair_record = set()
sequence = defaultdict(int)
for line in sys.stdin:
    try:
        vid1, vid2 = map(int, line.split()[:2])
    except:
        continue
    pair = frozenset({vid1, vid2})
    if pair in pair_record:
        continue
    else:
        pair_record.add(pair)
        sequence[vid1]+=1
        sequence[vid2]+=1


try:
    n = int(sys.argv[1])
    for v in range(n):
        k = sequence.get(v, 0)
        print('{} {}'.format(v, k))
except:
    items = sorted(sequence.items(), key=lambda x:x[0])
    for v, k in items:
        print('{} {}'.format(v, k))        
    