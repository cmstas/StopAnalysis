import time
import json

# from lumi_utils import RunLumis, getRunLumis
from itertools import groupby
from operator import itemgetter

def getIntersect(list1, list2):
    lsb1 = []
    lsb2 = []
    for ls in list1:
        lsb1 += range(ls[0],ls[-1]+1)
    for ls in list2:
        lsb2 += range(ls[0],ls[-1]+1)
    lsb = list(set(lsb1).intersection(lsb2))
    # lsb = [filter(lambda x: x in lsb1, [sublist]) for sublist in lsb2]

    list3 = []
    # i = 0
    for k, g in groupby(enumerate(lsb), lambda (i, x): i-x):
        grp = map(itemgetter(1), g)    
        list3.append([grp[0],grp[-1]])

    return list3

def getCommonSubset(json1, json2):
    intersection = {}
    for run, ranges1 in json1.iteritems():
        if not run in json2:
            print "run:", run, "skipped"
            continue
        else:
            ranges2 = json2[run]
            intersection[run] = getIntersect(ranges1, ranges2)

    return intersection


lumi_ran = "../StopLooper/logs/lumi_ran.txt"
lines = open(lumi_ran).readlines()

ls_list = {}
runjson = {}
for line in lines:
    line = line.split()
    if line[0] in ls_list:
        ls_list[line[0]].append(int(line[1]))
    else:
        ls_list[line[0]] = [int(line[1]),]

for run, lss in ls_list.iteritems():
    lsr = []
    for k, g in groupby(enumerate(lss), lambda (i, x): i-x):
        grp = map(itemgetter(1), g)    
        lsr.append([grp[0],grp[-1]])
    runjson[run] = lsr

print json.dumps(runjson)
