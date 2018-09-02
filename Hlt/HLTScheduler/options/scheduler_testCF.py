from Configurables import (HLTControlFlowMgr,
                            HLTEventLoopMgr,
                            AlgResourcePool,
                            ConfigurableDummy,
                            HiveWhiteBoard,
                            HiveDataBrokerSvc,
                            )
from Gaudi.Configuration import *


evtslots = 2
evtMax = 4
threads = 2

a1 = ConfigurableDummy("A1")
a1.outKeys = ['/Event/a1']
a1.CFD = True

a2 = ConfigurableDummy("A2")
a2.inpKeys = ['/Event/a1']
a2.outKeys = ['/Event/a2']
a2.CFD = False

a3 = ConfigurableDummy("A3")
a3.inpKeys = ['/Event/a1']
a3.outKeys = ['/Event/a3']
a3.CFD = False

a4 = ConfigurableDummy("A4")
a4.inpKeys = ['/Event/a3']
a4.outKeys = ['/Event/a4']
a4.CFD = True

a5 = ConfigurableDummy("A5")
a5.inpKeys = ['/Event/a4', '/Event/a2']
a5.outKeys = ['/Event/a5']
a5.CFD = True

whiteboard = HiveWhiteBoard("EventDataSvc",
                            EventSlots=evtslots)

HLTControlFlowMgr().CompositeCFNodes = [
             ( 'moore', 'NONLAZY_AND', ['line2', 'decision'], True ),
             ( 'decision', 'NONLAZY_OR', ['line1', 'A5', 'notA1'], False ),
             ( 'line1', 'LAZY_OR', ['A1', 'A2'], True ),
             ( 'line2', 'LAZY_AND', ['A3', 'A4'], True ),
             ( 'notA1', 'NOT', ['A1'], True ),
]
HLTControlFlowMgr().AdditionalCFEdges = [ ['A5', 'line1'] ]

HLTControlFlowMgr().ThreadPoolSize = threads
HLTControlFlowMgr().OutputLevel = DEBUG

HLTEventLoopMgr().ThreadPoolSize = threads
HLTEventLoopMgr().OutputLevel = DEBUG

HiveDataBrokerSvc().OutputLevel = DEBUG



app = ApplicationMgr(EvtMax=evtMax,
               EvtSel='NONE',
               ExtSvc=[whiteboard],
               EventLoop=HLTControlFlowMgr(),
               # EventLoop=HLTEventLoopMgr(),
               TopAlg=[a1, a2, a3, a4, a5])

HiveDataBrokerSvc().DataProducers = app.TopAlg
