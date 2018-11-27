#!/usr/bin/env python
###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
import argparse
import shelve
import sys
try:
    from TCKUtils.utils import matchInTree
except ImportError:
    print 'This script must be run in the HLT project environment.'
    sys.exit(-1)

parser = argparse.ArgumentParser(usage='usage: %(prog)s tck')

parser.add_argument("tck", help="what TCK")

args = parser.parse_args()

tck = int(args.tck, 16)


def properties(type, name, props):
    d = {}
    for m in matchInTree(tck, name, type=type):
        for pcfg in m.leafs().values():
            d[pcfg.type + '/' + pcfg.name] = {p: pcfg.props[p] for p in props}
    return d


db = shelve.open('0x%08x.props' % tck)

db['factories'] = properties('.*LoKi.*', '.*Factory.*', ['Modules'])
db['writers'] = properties('HltRoutingBitsWriter', '.*RoutingBitsWriter',
                           ['RoutingBits'])
db.close()
