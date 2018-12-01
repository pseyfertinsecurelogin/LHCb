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
from Gaudi.Configuration import *
from Configurables import LHCbApp, DataOnDemandSvc
from Configurables import GaudiSequencer
from Configurables import EventClockSvc

importOptions("$STDOPTS/DecodeRawEvent.py")

LHCbApp()
EventClockSvc(EventTimeDecoder = "OdinTimeDecoder")

mySeq=GaudiSequencer("Decoding")
mySeq.OutputLevel=VERBOSE

def append():
    for loc,alg in DataOnDemandSvc().AlgMap.iteritems():
        if ('UT' not in loc) and ('FT' not in loc) and ("VL" not in loc) and ("VP" not in loc):
            if alg not in mySeq.Members:
                 mySeq.Members.append(alg)

#make sure ODIN is first, to resolve ST dependencies manually...
append()
ApplicationMgr().TopAlg=[mySeq]
