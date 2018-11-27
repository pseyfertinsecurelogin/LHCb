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
from Configurables import (HLTControlFlowMgr,
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

a1 = ConfigurableDummy("A1")
a1.CFD = True

a2 = ConfigurableDummy("A2")
a2.CFD = False

whiteboard = HiveWhiteBoard("EventDataSvc",
                            EventSlots=evtslots)


nodecombinations = [x for x in itertools.product(['A1','A2'], repeat = 2)]

listofnodes = ['NONLAZY_AND', 'NONLAZY_OR', 'LAZY_AND', 'LAZY_OR']

combinations = [x for x in itertools.product(listofnodes, nodecombinations)]

HLTControlFlowMgr().CompositeCFNodes = [('top', 'NONLAZY_OR', [str(i) for i in range(len(combinations))] + ['notA1', 'notA2'], False)]
for i in range(len(combinations)):
    HLTControlFlowMgr().CompositeCFNodes.append(( str(i), combinations[i][0], combinations[i][1], False ))

HLTControlFlowMgr().CompositeCFNodes.append(('notA1', 'NOT', ['A1'], False))
HLTControlFlowMgr().CompositeCFNodes.append(('notA2', 'NOT', ['A2'], False))


HLTControlFlowMgr().ThreadPoolSize = threads
HLTControlFlowMgr().OutputLevel = DEBUG

HLTEventLoopMgr().ThreadPoolSize = threads
HLTEventLoopMgr().OutputLevel = DEBUG

HiveDataBrokerSvc().OutputLevel = DEBUG

print(HLTControlFlowMgr().CompositeCFNodes)


app = ApplicationMgr(EvtMax=evtMax,
               EvtSel='NONE',
               ExtSvc=[whiteboard],
               EventLoop=HLTControlFlowMgr(),
               TopAlg=[a1,a2])

HiveDataBrokerSvc().DataProducers = app.TopAlg
