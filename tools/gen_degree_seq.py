#!/usr/bin/env python3
import argparse
import sys
from collections import defaultdict

parser = argparse.ArgumentParser(description='Generate degree sequence')
parser.add_argument('-3c', '--three_column', dest='three_column', action='store_true', default=False,
                    help='use three-column format for the bivariate distribution')
args = parser.parse_args()

pair_record = set()
sequence = defaultdict(int)


if args.three_column:
    for line in sys.stdin:
        vid1, vid2, k = line.split()
        vid1, vid2, k = int(vid1), int(vid2), float(k)
        sequence[vid1]+=k
        sequence[vid2]+=k
else:
    for line in sys.stdin:
        vid1, vid2 = map(int, line.split())
        sequence[vid1]+=1
        sequence[vid2]+=1

try:
    n = 10
    for v in range(n):
        k = sequence[v]
        print('{} {}'.format(v, k))
except:
    for v, k in sequence.items():
        print('{} {}'.format(v, k))

