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
from Configurables import (
    HLTControlFlowMgr,
    HLTEventLoopMgr,
    AlgResourcePool,
    ConfigurableDummy,
    HiveWhiteBoard,
    HiveDataBrokerSvc,
)
from Gaudi.Configuration import *
import itertools

evtslots = 2
evtMax = 4
threads = 2

Ts = [ConfigurableDummy("T{}".format(i)) for i in range(8)]
Fs = [ConfigurableDummy("F{}".format(i)) for i in range(8)]

for t in Ts:
    t.CFD = True
for f in Fs:
    f.CFD = False

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)
HLTControlFlowMgr().CompositeCFNodes = []

listofnodes = ['NONLAZY_AND', 'NONLAZY_OR', 'LAZY_AND', 'LAZY_OR']

helperindex = 0
for compNode in listofnodes:
    #once with a True node first
    HLTControlFlowMgr().CompositeCFNodes.append(
        (compNode + "_TF", compNode,
         [Ts[helperindex].name(), Fs[helperindex].name()], True))
    helperindex += 1
    #once with a False node first
    HLTControlFlowMgr().CompositeCFNodes.append(
        (compNode + "_FT", compNode,
         [Fs[helperindex].name(), Ts[helperindex].name()], True))
    helperindex += 1

HLTControlFlowMgr().CompositeCFNodes.append(
    ('top', 'NONLAZY_OR', [x[0] for x in HLTControlFlowMgr().CompositeCFNodes],
     False))

HLTControlFlowMgr().ThreadPoolSize = threads
HLTControlFlowMgr().OutputLevel = VERBOSE

HLTEventLoopMgr().ThreadPoolSize = threads
HLTEventLoopMgr().OutputLevel = DEBUG

HiveDataBrokerSvc().OutputLevel = DEBUG

print(HLTControlFlowMgr().CompositeCFNodes)

app = ApplicationMgr(
    EvtMax=evtMax,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=HLTControlFlowMgr(),
    TopAlg=Ts + Fs)

HiveDataBrokerSvc().DataProducers = app.TopAlg
