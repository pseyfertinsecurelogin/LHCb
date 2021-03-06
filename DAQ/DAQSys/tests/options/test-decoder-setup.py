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
import DAQSys
from DAQSys.Decoders import DecoderDB
from DAQSys.DecoderClass import validate

validate(DecoderDB)

for k, v in DecoderDB.items():
    if v.Active:
        if len(v.listInputs()) == 0:
            raise ValueError("Decoder " + v.FullName +
                             " input location unknown.")
        for l in v.listInputs():
            if not len(l):
                raise ValueError("Zero length default for input " + v.FullName)
        if len(v.listOutputs()) == 0:
            raise ValueError("Decoder " + v.FullName +
                             " output location unknown.")
        for l in v.listOutputs():
            if not len(l):
                raise ValueError("Zero length default for output " +
                                 v.FullName)
        if len(v.Banks) == 0:
            raise ValueError("Decoder " + v.FullName + " banks unknown.")

#check the setup
for k, decoder in DecoderDB.items():
    if decoder.Active:
        decoder.setup()

#add an alg already into the DoD with the default name
dec = None
for k, decoder in DecoderDB.iteritems():
    if decoder.Active:
        if k.split('/')[0] == k.split('/')[-1]:
            dec = decoder
            break
if dec is None:
    raise KeyError("no defaultly named decoders exist :S")

from Configurables import DataOnDemandSvc
DataOnDemandSvc().AlgMap[dec.listOutputs()[0]] = dec.FullName

#check the configurable
from Configurables import DecodeRawEvent
DecodeRawEvent().OverrideInputs = "Hlt"
DecodeRawEvent().DataOnDemand = True
DecodeRawEvent().__apply_configuration__()

print "Pass"
