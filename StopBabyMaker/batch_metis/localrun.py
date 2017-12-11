import time
import os

with open('unfinished1.txt') as fin:
    lines = fin.readlines()
    os.system('cd input')
    for line in lines:
        if line[0] == "#": continue
        line = line.split()

        os.system('./runBabyMaker stopbaby -1 1 ./ {} 0 '.format(line[1]))
        os.system('cp stopbaby_1.root {}'.format(line[0]))

        time.sleep(5)
        # srlst = []
        # for f in line[1].split(','):
        #     srlst.append(f)
        # resub_samples[line[0]] = srlst

    
        
