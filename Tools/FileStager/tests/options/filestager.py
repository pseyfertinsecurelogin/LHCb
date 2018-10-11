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
from GaudiConf import IOHelper
from Configurables import LHCbApp, EventSelector
from Configurables import ApplicationMgr, GaudiSequencer
from Configurables import HltRoutingBitsFilter
from DAQSys.Decoders import DecoderDB
from Configurables import LoKiSvc
from PRConfig.TestFileDB import test_file_db
from FileStager.Configuration import configureFileStager

input_file = test_file_db["HltDAQ-routingbits_full"].filenames[0]

configureFileStager()

# General configuration
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2016'
app.CondDBtag = 'cond-20160517'
app.DDDBtag = 'dddb-20150724'

EventSelector().PrintFreq = 1000

topSeq = GaudiSequencer("TopSequence")

# Filter nanofied events if the file is HLT2 accepted
nano_filter = HltRoutingBitsFilter(
    'NonNanoFilter', RequireMask=[0x0, 0x0, 0x80000000])
topSeq.Members += [nano_filter]

# Decode Hlt DecReports
for dec in ("HltDecReportsDecoder/Hlt1DecReportsDecoder",
            "HltDecReportsDecoder/Hlt2DecReportsDecoder"):
    topSeq.Members.append(DecoderDB[dec].setup())
    # tell HltConfigSvc to leave the just explicitly configured decoders
    # alone..

ApplicationMgr().TopAlg = [topSeq]

LoKiSvc().Welcome = False

IOHelper("MDF").inputFiles([input_file])
