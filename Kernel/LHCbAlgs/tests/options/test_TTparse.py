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
from LHCbAlgs import TTParser

TT1 = TTParser.parse("../refs/timing.dat")
TT2 = TTParser.parse("../refs/timing.csv")

comp = TTParser.compare("../refs/timing.dat", "../refs/timing.csv")
if comp != (['TestAlg4'], ['TestAlg2', 'TestAlg3'], ['TestAlg5']):
    print "Found: ", comp
    print "Expected: ", (['TestAlg4'], ['TestAlg2', 'TestAlg3'], ['TestAlg5'])
    raise ValueError("compare produced the wrong results, cehck stdout")

print "PASS"
