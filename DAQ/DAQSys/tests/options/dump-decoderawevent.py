###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from Gaudi.Configuration import *
importOptions("../options/OldDecodeRawEvent.py")

from GaudiConf.Manipulations import configurableInstanceFromString

for k,v in configurableInstanceFromString("DataOnDemandSvc").AlgMap.iteritems():
    if type(v) is str:
        v=configurableInstanceFromString(v)
    print "===================="
    print k
    print "--------------------"
    print v.getFullName()
    props={}
    for prop in v.getProperties():
        props[prop]=v.getProp(prop)
    print props
    print "===================="
