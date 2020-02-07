###############################################################################
# (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from Configurables import (CondDB, LHCbApp, HLTControlFlowMgr, createODIN,
                           HiveDataBrokerSvc, UpdateManagerSvc, HiveWhiteBoard)
from Configurables import Gaudi__Hive__FetchDataFromFile as FetchDataFromFile
from Configurables import LHCb__DetDesc__ReserveDetDescForEvent as ReserveIOV
from PRConfig.TestFileDB import test_file_db
from Gaudi.Configuration import ERROR

app = LHCbApp()
app.EvtMax = 100
app.EnableHive = True
app.Scheduler = "HLTControlFlowMgr"
app.ThreadPoolSize = 1
test_file_db['MiniBrunel_2018_MinBias_FTv4_DIGI'].run(configurable=app)

CondDB().Upgrade = True
UpdateManagerSvc(WithoutBeginEvent=True)
HiveWhiteBoard("EventDataSvc", EventSlots=1)
HiveDataBrokerSvc(
    OutputLevel=ERROR,
    DataProducers=[
        FetchDataFromFile('ReadRawEvent', DataKeys=['/Event/DAQ/RawEvent']),
        createODIN(),
        ReserveIOV()
    ])
HLTControlFlowMgr(
    MemoryPoolSize=512 * 1024,  # 512 KiB
    CompositeCFNodes=[('muon_decoding', 'LAZY_AND', ['MuonRawToHits'], True)])
