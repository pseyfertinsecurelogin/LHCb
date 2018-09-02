from Configurables import (HLTControlFlowMgr,
                            AlgResourcePool,
                            ConfigurableDummy,
                            HiveWhiteBoard,
                            HiveDataBrokerSvc,
                            HLTEventLoopMgr,
                            CallgrindProfile,
                            CPUCruncher,
                            )
from Gaudi.Configuration import *

from random import random

#configuration
useDummies = True
nlines = 10
linedepth = 1 #TODO implement this shit
linespread = 5 #number of direct children per line, total number of basicnodes is linespread^linedepth
nDataProducers = 5
nodeAcceptance = 1
runtime = 3e-3/50 #not for dummies
varruntime = 0 #not for dummies

#computing configuration
enableCallgrind = False

if enableCallgrind:
    evtslots = 1
    evtMax = 1100
    threads = 1
else:
    evtslots = 60
    evtMax = 3000000
    threads = 40


if useDummies:
    baseUnit = ConfigurableDummy
else: 
    baseUnit = CPUCruncher


#concurrency conf
HLTControlFlowMgr().ThreadPoolSize = threads
HLTEventLoopMgr().ThreadPoolSize = threads

whiteboard = HiveWhiteBoard("EventDataSvc",
                            EventSlots=evtslots)

if enableCallgrind:
    p = CallgrindProfile()
    p.StartFromEventN = 100
    p.StopAtEventN = evtMax
    # p.DumpAtEventN = p.StopAtEventN

#create nDataProducers Producers (to be used in every node)
DPs = []
if nDataProducers > 0:
    DPs.append(baseUnit('DP0'))
    DPs[0].outKeys = [DPs[0].name()]
    for i in range(1, nDataProducers):
        DPs.append(baseUnit('DP{}'.format(i)))
        DPs[i].inpKeys = DPs[i-1].outKeys
        DPs[i].outKeys = [DPs[i].name()]
        DPs[i].Cardinality = 0

#create Lines
CNodeConfs = [('decision', 'NONLAZY_OR', ['L{}'.format(i) for i in range(nlines)], False)]
for i in range(nlines):
    CNodeConfs.append(('L{}'.format(i), 'LAZY_AND', ['L{}C{}'.format(i,j) for j in range(linespread)], True))

if enableCallgrind:
    HLTControlFlowMgr().AdditionalCFEdges = [[p.name(), i] for i in CNodeConfs[0][2]]
    CNodeConfs[0][2].insert(0,p.name()) #insert callgrindprofile to the decision


#get all CNode names
CNodeNames = [CNode[0] for CNode in CNodeConfs]

#infer BasicNode names
BNodesNames = list({x for CNode in CNodeConfs for x in CNode[2] if x not in CNodeNames})

if enableCallgrind:
    BNodesNames.remove(p.name())

#build BasicNodes
BNodes = [baseUnit('{}'.format(BNodeName)) for BNodeName in BNodesNames]
print(BNodes)

#define their input
for i in range(len(BNodes)):
    if nDataProducers > 0:
        BNodes[i].inpKeys = [ DPs[-1].name() ]
    if useDummies:
        BNodes[i].CFD = random() <= nodeAcceptance
    else:
        BNodes[i].InvertDecision = not (random() <= nodeAcceptance)
    BNodes[i].Cardinality = 0

if not useDummies:
    for k in BNodes + DPs:
            k.avgRuntime = runtime
            k.varRuntime = varruntime
            k.shortCalib = True

#configure HLTControlFlowMgr
HLTControlFlowMgr().CompositeCFNodes = CNodeConfs
# HLTControlFlowMgr().OutputLevel = DEBUG
#HiveDataBrokerSvc().OutputLevel = DEBUG


app = ApplicationMgr(EvtMax=evtMax,
               EvtSel='NONE',
               ExtSvc=[whiteboard],
               # EventLoop=HLTEventLoopMgr(),
               EventLoop=HLTControlFlowMgr(),
               TopAlg=DPs + BNodes)

if enableCallgrind:
    app.TopAlg.insert(0,p.name())

HiveDataBrokerSvc().DataProducers = app.TopAlg
