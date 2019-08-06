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
    HiveWhiteBoard,
    HiveDataBrokerSvc,
)
from Gaudi.Configuration import *
from Configurables import Gaudi__Examples__IntDataProducer as IDP
from Configurables import Gaudi__Examples__FloatDataConsumer as FDC

one = IDP('one', OutputLocation="stuff")
two = FDC('two', InputLocation="stuff")

evtslots = 2
evtMax = 100
threads = 2

whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=evtslots)

HLTControlFlowMgr().CompositeCFNodes = [
    ('moore', 'LAZY_AND', ['two'], True),
]

HLTControlFlowMgr().ThreadPoolSize = threads

app = ApplicationMgr(
    EvtMax=evtMax,
    EvtSel='NONE',
    ExtSvc=[whiteboard],
    EventLoop=HLTControlFlowMgr(),
    TopAlg=[one, two])

HiveDataBrokerSvc().DataProducers = app.TopAlg
