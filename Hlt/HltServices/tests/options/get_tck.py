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
# General configuration
import os
import re
from GaudiConf import IOHelper
from Gaudi.Configuration import *
from Configurables import LHCbApp
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2015'
# app.Simulation = True

from Configurables import CondDB
CondDB().LatestGlobalTagByDataTypes = [app.DataType]
CondDB().Simulation = False

from Configurables import Gaudi__IODataManager as IODataManager
IODataManager().DisablePFNWarning = True

# from PRConfig.TestFileDB import test_file_db
# input = test_file_db['2012_raw_default']
# input = test_file_db['MC2015_MinBias_SPD_lt_420_md_4xKee_L0Filtered']
# input.run(configurable = app)

# from PRConfig.TestFileDB import test_file_db
# input = test_file_db['2012_raw_default']
# input.run(configurable = app)

# base_dir = '/daqarea/lhcb/data/2016/RAW/TURBO/LHCb/COLLISION16'
# base_dir = '/localdisk/hlt1/data2015/0506NB'
# base_dir = '/net/hlta0405/localdisk/Alignment/BWDivision'
# base_dir = '/net/hlt%s%02d%02d/localdisk/hlt1'
# runnrs = [157809]
# re_file = re.compile(r"(?:Run_)?(0*(%s)).*\.(mdf|raw)" %
#                      ('|'.join((str(r) for r in runnrs))))
# from itertools import product
# files = []
# for run in sorted(runnrs):
#     if base_dir.startswith("/net"):
#         for node in product(['a'], range(1, 6), range(1, 29)):
#             bd = base_dir % node
#             file_dir = bd
#             if not os.path.exists(bd):
#                 continue
#             if str(run) in os.listdir(bd):
#                 file_dir = os.path.join(bd, str(run))
#             files += sorted([os.path.join(file_dir, f)
#                              for f in os.listdir(file_dir) if re_file.match(f)])
#     else:
#         file_dir = base_dir
#         if str(run) in os.listdir(base_dir):
#             file_dir = os.path.join(base_dir, str(run))
#         files += sorted([os.path.join(file_dir, f)
#                          for f in os.listdir(file_dir) if re_file.match(f)])

from PRConfig.TestFileDB import test_file_db
files = test_file_db["HltServices-close_cdb_file"].filenames

IOHelper("MDF", "MDF").inputFiles(files)

# Top level sequence
from Configurables import createODIN
from Configurables import bankKiller
hlt_banks = ['HltRoutingBits']
killer = bankKiller('RemoveRoutingBits', BankTypes=hlt_banks)

topSeq = GaudiSequencer("TopSequence")
# topSeq.Members = [createODIN(), killer]

from Configurables import HltRoutingBitsFilter
physFilter = HltRoutingBitsFilter("PhysFilter")
physFilter.RequireMask = [0x0, 0x4, 0x0]

topSeq.Members = [physFilter]

# from DAQSys.Decoders import DecoderDB
# for dec in ("HltDecReportsDecoder/Hlt1DecReportsDecoder",
#             "HltSelReportsDecoder/Hlt1SelReportsDecoder"):
#     topSeq.Members.append(DecoderDB[dec].setup())

ApplicationMgr().TopAlg = [topSeq]

# Timing table to make sure things work as intended
from Configurables import AuditorSvc, LHCbTimingAuditor
ApplicationMgr().AuditAlgorithms = 1
if 'AuditorSvc' not in ApplicationMgr().ExtSvc:
    ApplicationMgr().ExtSvc.append('AuditorSvc')
AuditorSvc().Auditors.append(LHCbTimingAuditor(Enable=True))

from Configurables import LoKiSvc
LoKiSvc().Welcome = False

from GaudiPython import AppMgr
gaudi = AppMgr()
gaudi.initialize()

TES = gaudi.evtSvc()

tcks = set()

while True:
    gaudi.run(1)
    if not TES['/Event']:
        break

    odin = TES['DAQ/ODIN']
    tcks.add(odin.triggerConfigurationKey())

print tcks
