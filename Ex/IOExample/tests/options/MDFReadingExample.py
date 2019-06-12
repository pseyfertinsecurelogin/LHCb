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
from Gaudi.Configuration import ApplicationMgr
from Configurables import LHCb__MDF__IOSvcMM, HiveWhiteBoard, HLTControlFlowMgr, UpdateManagerSvc, HiveDataBrokerSvc, DDDBConf, CondDB, LHCb__MDF__IOAlg
from PRConfig.TestFileDB import test_file_db
from GaudiConf import IOHelper
import os

# setup core Gaudi
whiteboard = HiveWhiteBoard("EventDataSvc", EventSlots=5, ForceLeaves=True)
eventloopmgr = HLTControlFlowMgr(
    "HLTControlFlowMgr",
    CompositeCFNodes=[('moore', 'LAZY_AND', [],
                       True)],  #sequence to be filled by defineSequence
    ThreadPoolSize=5)
appMgr = ApplicationMgr(
    'ApplicationMgr', EventLoop=eventloopmgr, EvtMax=1000, EvtSel='NONE')
appMgr.ExtSvc.insert(0, whiteboard)
uptMgr = UpdateManagerSvc('UpdateManagerSvc', WithoutBeginEvent=True)
hiveDataBroker = HiveDataBrokerSvc('HiveDataBrokerSvc')

# deal with input files
fileDbEntry = test_file_db["MiniBrunel_2018_MinBias_FTv4_MDF"]
qualifiers = fileDbEntry.qualifiers
iohelper = IOHelper("MDF", None)
iohelper.setupServices()
dddb = DDDBConf(
    'DDDBConf',
    Simulation=qualifiers['Simulation'],
    DataType=qualifiers['DataType'])
conddb = CondDB(
    'CondDB',
    Upgrade=True,
    Tags={
        "DDDB": qualifiers['DDDB'],
        "SIMCOND": qualifiers['CondDB']
    })

path = "/tmp/00067189.mdf"
if not os.path.isfile(path):
    os.system(
        "xrdcp -s root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/swtest/lhcb/swtest/MiniBrunel/00067189.mdf %s"
        % path)
files = [path] * 10

# define algorithms and service we will use for that example
mdfioSvc = LHCb__MDF__IOSvcMM('LHCb::MDF::IOSvcMM', Input=files)
fetchData = LHCb__MDF__IOAlg(
    'ReadMDFInput',
    RawEventLocation="/Event/DAQ/RawEvent",
    IOSvc="LHCb::MDF::IOSvcMM")
hiveDataBroker.DataProducers.append(fetchData)
appMgr.TopAlg.append(fetchData)

# and build the sequence
appMgr.EventLoop.CompositeCFNodes[0][2].extend([fetchData.name()])
