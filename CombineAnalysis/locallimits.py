#! /usr/bin/python

# Multiprocessing example taken from: https://github.com/cmstas/SSAnalysis/blob/master/cards/limitsTable_general.py

import os, sys, ROOT, array, re
from multiprocessing.dummy import Pool as ThreadPool
import time, random

have_tqdm = False
verbose = True
dosignificances = False
dolimits = True

carddir = 'datacards/scan_temp13'
combineddir = carddir + '/combinedcards'
limitdir = 'limits/'+carddir.split('/')[-1]


os.nice(10)

def combine_cards(sig):
    os.system('./CombineCards.sh {} {}/'.format(sig, carddir))
    return combineddir+'/datacard_'+sig+'.txt'


def run_asymptotic(card):
    # stagger the processes so that they don't all do createCards
    # at the same time, otherwise histograms get corrupted when
    # writing the statUp statDown crap
    time.sleep(30.0*random.random())

    if verbose:
        print 'Running limit on ', card

    # if dosignificances:
    #     os.system("timeout 15m combine -M ProfileLikelihood --uncapped 1 --significance --rMin -5 "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.txt >& "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all_significance.log || echo 'Triggered timeout of 15m'")

    if dolimits:
        if not os.path.isfile(card): return -1
        # os.system(cmd)
        logname = card[:-4]+"_asym.log"
        masspt = 'PreFit_'+re.findall(r'datacard_([A-Za-z0-9_\./\\-]*).txt', card)[0]
        cmdstr = 'combine -M Asymptotic '+card+' -n '+masspt
        os.system("timeout 25m "+ cmdstr +" >& "+ logname +" || echo 'Triggered timeout of 25m'") # --run expected --noFitAsimov
        os.system("mv higgsCombine{0}.Asymptotic.mH120.root {1}/Limits_Asymptotic_{0}.root".format(masspt,limitdir)) # --run expected --noFitAsimov
        return logname


if __name__ == "__main__":

    if len(sys.argv) > 1:
        carddir = sys.argv[1]
        combineddir = carddir + '/combinedcards'
        limitdir = 'limits/'+carddir.split('/')[-1]

    print "Doing limits from cards in ", carddir

    ext_cards = os.listdir(carddir)
    ext_cards = filter(lambda x : "std_T2tt" in x and "bin1.txt" in x, ext_cards)

    sigs = [ x[9:-9] for x in ext_cards]
    # print sigs

    pool = ThreadPool(20)
    cards = []

    for combined in pool.imap_unordered(combine_cards, sigs):
        cards.append(combined)

    # print cards

    os.system('mkdir -p '+limitdir)
    limits = []
    for result in pool.imap_unordered(run_asymptotic, cards):
        limits.append(result)

    print limits

    pool.close()
    pool.join()
