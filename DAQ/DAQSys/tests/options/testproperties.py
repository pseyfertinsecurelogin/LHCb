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
from DAQSys.Decoders import DecoderDB
l0du = DecoderDB["L0DUDecoder/L0DUFromRaw"]
l0du.Properties["SpammySpamSpam"] = False

wasset = []
for k, d in DecoderDB.iteritems():
    if d == l0du:
        continue
    if "SpammySpamSpam" in d.Properties:
        wasset.append(k)

if len(wasset):
    raise KeyError("Setting a property also accidentally set a property for" +
                   wasset.__str__())

print "Pass"
