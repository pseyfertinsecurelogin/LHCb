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
from Configurables import DecodeRawEvent, LHCbApp
from Configurables import GaudiSequencer
from Configurables import EventClockSvc

LHCbApp()
#EventClockSvc(EventTimeDecoder = "OdinTimeDecoder")

mySeq=GaudiSequencer("Decoding")
mySeq.OutputLevel=VERBOSE
DecodeRawEvent().Sequencer=mySeq
ApplicationMgr().TopAlg=[mySeq]

#deactivate Upgrade banks

from DAQSys.Decoders import DecoderDB
[DecoderDB[k].deactivate()  for k in DecoderDB if (DecoderDB[k].Active and (('createODIN' in k) or ('FT' in k) or ('UT' in k) or ('VL' in k) or ('VP' in k)))]
print [k for k in DecoderDB if (DecoderDB[k].Active)]

#configure L0TCKs
importOptions('$L0TCK/L0DUConfig.opts')
