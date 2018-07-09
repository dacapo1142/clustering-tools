import fileinput
from collections import Counter

raw_data = list(fileinput.input())
fileinput.close()
data = [set(map(int, line.split())) for line in raw_data]
from functools import reduce
import operator
count=sum(map(Counter, data), Counter())

remove_duplicate = [set(filter(lambda x:count[x]<=1, line)) for line in data]
remove_duplicate = list(filter(bool, remove_duplicate))
remove_lessthenthree = list(filter(lambda x:len(x)>=3, remove_duplicate))


total=reduce(set.union, remove_lessthenthree, set())

sorted_total=sorted(total)

f=open('top{}.cmty'.format(len(remove_lessthenthree)),'w')
for s in remove_lessthenthree:
    print(' '.join(map(str,s)), file=f)
f.close()

f=open('vid{}.list'.format(len(sorted_total)),'w')
print('\n'.join(map(str,sorted_total)), file=f)
f.close()