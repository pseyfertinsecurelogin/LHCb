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
from DAQSys.DecoderClass import Decoder, decodersForBank
from DAQSys.Decoders import DecoderDB

rich = decodersForBank(DecoderDB, "Rich")
required = []
for r in rich:
    for req in r.allDaughters():
        req = DecoderDB[req]
        if req not in required and req not in rich:
            required.append(req)

#push into a smaller DB

db = {}

for r in rich + required:
    r.__db__ = db
    db[r.FullName] = r

r0 = rich[0]
r0.overrideOutputs("Dev/Null")

expectname = "Rich::Future::RawBankDecoder/DecodeRawRichOffline"
prop = "DumpRawBanks"

rclone = r0.clone(expectname)
rclone.Properties[prop] = True
r0.Properties[prop] = False

rcloneset = rclone.setup()

r0set = r0.setup()

if rcloneset.getFullName() != expectname:
    raise NameError("Name set for the cloned algorithm is wrong!")

if r0.__db__ != rclone.__db__ or r0.__db__ != db:
    raise AttributeError("Dictionaries somehow became different on cloning")

if (not rcloneset.getProp(prop)) or r0set.getProp(prop):
    print rcloneset.getProp(prop), r0set.getProp(prop)
    raise ValueError("Cloning failed to produce different properties!")

if rcloneset.getProp(rclone.Outputs.keys()[0]) != r0set.getProp(
        r0.Outputs.keys()[0]) or r0set.getProp(
            r0.Outputs.keys()[0]) != "Dev/Null":
    print rcloneset.getProp(rclone.Outputs.keys()[0]), r0set.getProp(
        r0.Outputs.keys()[0])
    raise ValueError("Cloning failed to preserve output location")

print "Pass"
