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

LHCbApp()

mySeq=GaudiSequencer("Decoding")
mySeq.OutputLevel=VERBOSE
DecodeRawEvent().Sequencer=mySeq
ApplicationMgr().TopAlg=[mySeq]

from DAQSys.DecoderClass import decodersForBank
from DAQSys.Decoders import DecoderDB as ddb

#only calo banks
for k,v in ddb.iteritems():
    v.Active=False

flagged=[]

for b in ["PrsE", "EcalE", "HcalE", "ODIN"]:
    for d in decodersForBank(ddb,b,ignoreActive=True,addRequired=True):
        flagged.append(d)

for k,v in ddb.iteritems():
    if v not in flagged:
        v.Active=False



#configure L0TCKs
importOptions('$L0TCK/L0DUConfig.opts')

