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
from Gaudi.Configuration import *
from Configurables import DecodeRawEvent

from DAQSys.Decoders import DecoderDB

for i, v in DecoderDB.iteritems():
    v.Properties["OutputLevel"] = VERBOSE
    if "Hlt" in i and "ReportsDecoder" in i:
        v.Active = False
        #v.Inputs={"InputRawEventLocation":"DAQ/RawEvent"}
    for b in ["UT", "FT", "FTCluster", "VP", "VL"]:
        if b in v.Banks:
            v.Active = False

DecoderDB["MuonRec"].Active = False
DecoderDB["createODIN"].Active = False
#DecoderDB["OdinTimeDecoder/ToolSvc.OdinTimeDecoder"].Active=True
from Configurables import EventClockSvc, OdinTimeDecoder, ODINDecodeTool
ecs = EventClockSvc()
ecs.addTool(OdinTimeDecoder, 'EventTimeDecoder')
ecs.EventTimeDecoder.addTool(ODINDecodeTool)
ecs.EventTimeDecoder.ODINDecodeTool.RawEventLocations = ['Crazy/RawEvent']

DecodeRawEvent().OverrideInputs = 999
from Configurables import GaudiSequencer
#DecodeRawEvent().Sequencer=GaudiSequencer("SPAM")

for i, v in DecoderDB.iteritems():
    if v.Active:
        GaudiSequencer("SPAM").Members.append(v.setup())

from Configurables import StoreExplorerAlg

StoreExplorerAlg().Load = True

ApplicationMgr().TopAlg = [GaudiSequencer("SPAM"), StoreExplorerAlg()]
