from Configurables import ApplicationMgr, LHCbApp, HiveWhiteBoard, EventDataSvc
app = ApplicationMgr()

from Configurables import Gaudi__Examples__IntDataProducer, Gaudi__Examples__VectorDataProducer, Gaudi__Examples__KeyedDataProducer
produce_to_rootintes = Gaudi__Examples__KeyedDataProducer("KeyedDataProducerRootInTes",
        OutputLocation = "/Event/myrootintes/MyKeyed2")

from Configurables import LoKi__VoidFilter
filter_keyed = LoKi__VoidFilter("FilterKeyed",
        Code = "SIZE('MyKeyed')>1")
filter_keyed_rootintes = LoKi__VoidFilter("FilterKeyedRootInTes",
        Code = "SIZE('MyKeyed2')>1",
        RootInTES = "myrootintes")
filter_int = LoKi__VoidFilter("FilterInt",
        Code = "SIZE('MyInt')>1")
filter_vector = LoKi__VoidFilter("FilterVector",
        Code = "SIZE('MyVector')>1")
filter_old_keyed = LoKi__VoidFilter("FilterOldKeyed",
        Code = "CONTAINS('MyKeyed')>1")


# add producers
app.TopAlg += [Gaudi__Examples__IntDataProducer(),  Gaudi__Examples__VectorDataProducer(),
               produce_to_rootintes, Gaudi__Examples__KeyedDataProducer()]
# add consumers
app.TopAlg += [
    filter_keyed,
    filter_keyed_rootintes,
    filter_old_keyed,
    filter_int,
    filter_vector
    ]

EventDataSvc().ForceLeaves = True

# To test HiveMode and print data dependencies
# from Configurables import HiveWhiteBoard
# whiteboard   = HiveWhiteBoard("EventDataSvc")
# whiteboard.EventSlots = 1
# whiteboard.ForceLeaves = True

# ApplicationMgr().ExtSvc.insert(0, whiteboard)
# from Configurables import HLTEventLoopMgr
# eventloopmgr = HLTEventLoopMgr()
# eventloopmgr.ThreadPoolSize = 1
# ApplicationMgr().EventLoop = eventloopmgr
# from Configurables import UpdateManagerSvc
# UpdateManagerSvc().WithoutBeginEvent = True

# Needed for LoKi to decode functors
from Configurables import AlgContextSvc
AlgContextSvc(BypassIncidents=True)
app.EvtMax = 2
app.EvtSel = "NONE"
app.HistogramPersistency = "NONE"
