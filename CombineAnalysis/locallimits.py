#! /usr/bin/python

# Multiprocessing example taken from: https://github.com/cmstas/SSAnalysis/blob/master/cards/limitsTable_general.py

import os, sys, ROOT, array, re
from multiprocessing.dummy import Pool as ThreadPool
import time, random
import argparse

have_tqdm = False
verbose = True
dosignif = True
dolimits = True

os.nice(10)

def combine_cards(sig):
    os.system('./CombineCards.sh {} {}/'.format(sig, carddir))
    return combineddir+'/datacard_'+sig+'.txt'


def run_asymptotic(card):
    # stagger the processes so that they don't all do createCards
    # at the same time, otherwise histograms get corrupted when
    # writing the statUp statDown crap
    time.sleep(30.0*random.random())

    if not os.path.isfile(card): return -1

    if verbose:
        if dolimits and dosignif:
            print 'Running limit and significances on ', card
        elif dolimits:
            print 'Running limit on ', card
        elif dosignif:
            print 'Running significances on ', card

    if dosignif:
        logname = card[:-4]+"_signif.log"
        masspt = 'Observed_'+re.findall(r'datacard_([A-Za-z0-9_\./\\-]*).txt', card)[0]
        cmdstr = 'combine -M Significance '+card+' -n '+masspt+' --uncapped 1 --rMin -50 --rMax 50'
        # os.system("echo "+cmdstr)
        os.system("timeout 15m "+ cmdstr +" >& "+ logname +" || echo 'Job failed for '"+card+", see log at "+logname) # --run expected --noFitAsimov
        os.system("mv higgsCombine{0}.Significance.mH120.root {1}/Significance_{0}.root".format(masspt,signifdir)) # --run expected --noFitAsimov

        masspt = 'Expected_'+re.findall(r'datacard_([A-Za-z0-9_\./\\-]*).txt', card)[0]
        cmdstr = 'combine -M Significance '+card+' -n '+masspt+' --uncapped 1 --rMin -50 --rMax 50 -t -1 --expectSignal=1'
        os.system("timeout 15m "+ cmdstr +" >> "+ logname +" || echo 'Job failed for '"+card+", see log at "+logname) # --run expected --noFitAsimov
        os.system("mv higgsCombine{0}.Significance.mH120.root {1}/Significance_{0}.root".format(masspt,signifdir)) # --run expected --noFitAsimov

    if dolimits:
        logname = card[:-4]+"_asym.log"
        masspt = 'PostFit_'+re.findall(r'datacard_([A-Za-z0-9_\./\\-]*).txt', card)[0]
        cmdstr = 'combine -M AsymptoticLimits '+card+' -n '+masspt
        # cmdstr = 'combine -M Asymptotic --freezeNuisances all '+card+' -n '+masspt
        os.system("timeout 25m "+ cmdstr +" >& "+ logname +" || echo 'Job failed for '"+card+", see log at "+logname) # --run expected --noFitAsimov
        os.system("mv higgsCombine{0}.AsymptoticLimits.mH120.root {1}/Limits_Asymptotic_{0}.root".format(masspt,limitdir)) # --run expected --noFitAsimov
        return logname


if __name__ == "__main__":

    parser = argparse.ArgumentParser('Run limits locally')

    parser.add_argument('carddir')
    parser.add_argument('sigscan', nargs='?', default='std_T2tt')
    parser.add_argument('--runsignif', action='store_true', default=False)
    parser.add_argument('--nolimits', action='store_true', default=False)

    args = parser.parse_args()

    carddir = args.carddir
    combineddir = carddir + '/combinedcards'
    limitdir = 'limits/'+carddir.split('/')[-1]
    signifdir = 'significances/'+carddir.split('/')[-1]

    sigscan = args.sigscan
    dosignif = args.runsignif
    dolimits = not args.nolimits
    # print 'sigscan =', sigscan, ', dosignif =', dosignif, ', dolimits =', dolimits

    print "Doing limits from cards in ", carddir

    ext_cards = os.listdir(carddir)
    ext_cards = filter(lambda x : sigscan in x and "bin1.txt" in x, ext_cards)

    sigs = [ x[9:-9] for x in ext_cards]
    sigs = [ s for s in sigs if int(s.split('_')[2]) < 1500]
    # sigs = [ s for s in sigs if (int(s.split('_')[2])-int(s.split('_')[3])) > 250]

    pool = ThreadPool(40)
    cards = []

    for combined in pool.imap_unordered(combine_cards, sigs):
        cards.append(combined)

    # exit()

    if not cards:
        cards = os.listdir(combineddir)
        cards = filter(lambda x : '.txt' in x and '.log' not in x, cards)
        cards = [combineddir+'/'+c for c in cards]
        # print cards

    if dolimits: os.system('mkdir -p '+limitdir)
    if dosignif: os.system('mkdir -p '+signifdir)
    limits = []
    for result in pool.imap_unordered(run_asymptotic, cards):
        limits.append(result)

    print limits

    pool.close()
    pool.join()
