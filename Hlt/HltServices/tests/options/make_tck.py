import os
import argparse
from Configurables import HltGenConfig
from Configurables import GaudiSequencer
from Configurables import DeterministicPrescaler
from Configurables import ConfigCDBAccessSvc
from Configurables import ApplicationMgr
from Configurables import LHCbApp
from Configurables import CondDB


parser = argparse.ArgumentParser(usage='usage: %(prog)s prescale')

parser.add_argument("prescale", type=float, help="what prescale")

args = parser.parse_args()

scale = float(args.prescale)

app = LHCbApp()
app.EvtMax = -1
app.DataType = '2016'

CondDB().UseLatestTags = [app.DataType, True]

# Location of TCK database
TCKData = 'TCKData'
if not os.path.exists(TCKData):
    os.makedirs(TCKData)

# TCK access service
accessSvc = ConfigCDBAccessSvc(File=TCKData + '/config.cdb',
                               Mode='ReadWrite')

# Sequence, actually only a prescaler
seq = GaudiSequencer("TestSequence")
prescaler = DeterministicPrescaler("TestScaler",
                                   AcceptFraction=scale)
seq.Members = [prescaler]

# Algorithm to generate the TCK
gen = HltGenConfig(ConfigTop=[seq.getName()],
                   ConfigSvc=['ToolSvc'],
                   ConfigAccessSvc=accessSvc.getName(),
                   HltType="LHCb_Test",
                   MooreRelease="v1r0",
                   Label="Test")


# make sure gen is the very first Top algorithm
ApplicationMgr().TopAlg = [gen.getFullName(), seq]

# Instantiate AppMgr and run some events
from GaudiPython.Bindings import AppMgr

gaudi = AppMgr()
TES = gaudi.evtSvc()

gaudi.initialize()
gaudi.start()
gaudi.stop()
gaudi.finalize()
gaudi.exit()

print 'PASSED'
