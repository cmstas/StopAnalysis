#!/usr/bin/env python
from __future__ import print_function, division
import os, sys
import yaml

if __name__ == '__main__':

    
    # f = open('xsecs_inclusive.yml', 'r')
    f = open('xsec_dilepton_2016.yml', 'r')
        
    xsd = yaml.load(f)
    print(len(xsd))

    of = open('ttbarDM_xsecs_inclusive.txt','w')
    of.write('# spin mChi mPhi xsec xsec_unc')

    for itm in xsd:
        xsec = itm['xsec'] * 9.526
        print(itm['mChi'], itm['mPhi'], itm['spin'], xsec)
        spin = 0 if itm['spin'] == 'scalar' else 1 if itm['spin'] == 'pseudoscalar' else 2
        of.write('{} {} {} {} {}'.format(spin, itm['mChi'], itm['mPhi'], xsec, itm['xsec_unc']))


