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
import os
import argparse
from Configurables import HltGenConfig
from Configurables import GaudiSequencer
from Configurables import DeterministicPrescaler
from Configurables import ConfigCDBAccessSvc
from Configurables import ApplicationMgr
from Configurables import LHCbApp
from Configurables import CondDB
from Configurables import HltLinePersistenceSvc


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

# The HltLinePersistenceSvc (for the TCKLinePersistenceSvc test)
execfile(os.path.expandvars('$HLTSERVICESROOT/tests/options/'
                            'line_persistence_svc_cfg.py'))
persistence_svc = configure_hlt_svc()
ApplicationMgr().ExtSvc += [persistence_svc]

# Algorithm to generate the TCK
gen = HltGenConfig(ConfigTop=[seq.getName()],
                   ConfigSvc=['ToolSvc', persistence_svc.getFullName()],
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
