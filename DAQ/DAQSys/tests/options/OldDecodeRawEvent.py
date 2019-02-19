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
"""# INFO: ===================================================================
# INFO: this is the old DecodeRawEvent.py script, this is completely obsolete
# INFO: a copy is maintained here for some tests just to check that some of
# INFO: the configuration is the same from the old->new way of doing stuff
# INFO: but this will gradually become unmaintainable and useless
# INFO: ==================================================================="""

# Job options to configure the data on demand service for decoding the RawEvent
print "# WARNING: You have imported DecodeRawEvent.py, there is now a configurable doing the same job. See savannah task #19106."

print __doc__

from Gaudi.Configuration import *
from Configurables import (DataOnDemandSvc, DecodeVeloRawBuffer,
                           RawBankToSTClusterAlg, DecodeVeloRawBuffer,
                           RawBankToSTLiteClusterAlg, FTRawBankDecoder)
# for the upgrade
#VPLiteClusters = VPRawBankToLiteCluster("createVPLiteClusters")
#VPLiteClusters.ClusterLocation  = "Raw/VP/LiteClusters"
#VPClusters     = VPRawBankToPartialCluster("createVPClusters")
#VPClusters.ClusterLocation = "Raw/VP/Clusters"
#
FTClusters = FTRawBankDecoder("createFTClusters")
# Set up the algorithms
#DataOnDemandSvc().AlgMap["Raw/VP/Clusters"]       = VPClusters
#DataOnDemandSvc().AlgMap["Raw/VP/LiteClusters"]   = VPLiteClusters
DataOnDemandSvc(
).AlgMap["Raw/Velo/Clusters"] = "DecodeVeloRawBuffer/createVeloClusters"
DataOnDemandSvc(
).AlgMap["Raw/TT/Clusters"] = "RawBankToSTClusterAlg/createTTClusters"
DataOnDemandSvc(
).AlgMap["Raw/IT/Clusters"] = "RawBankToSTClusterAlg/createITClusters"
DataOnDemandSvc().AlgMap[
    "Raw/Velo/LiteClusters"] = "DecodeVeloRawBuffer/createVeloLiteClusters"
DataOnDemandSvc().AlgMap[
    "Raw/TT/LiteClusters"] = "RawBankToSTLiteClusterAlg/createTTLiteClusters"
DataOnDemandSvc().AlgMap[
    "Raw/IT/LiteClusters"] = "RawBankToSTLiteClusterAlg/createITLiteClusters"
DataOnDemandSvc().AlgMap["Raw/OT/Times"] = "OTTimeCreator"

#Error in original script! This was actually decoding somewhere else!
#DataOnDemandSvc().AlgMap["Raw/FT/RawClusters"]    = FTClusters
DataOnDemandSvc().AlgMap["Raw/FT/LiteClusters"] = FTClusters
DataOnDemandSvc().AlgMap["Raw/Muon/Coords"] = "MuonRec"
DataOnDemandSvc().AlgMap["DAQ/ODIN"] = "createODIN"
DataOnDemandSvc().AlgMap[
    "Raw/Rich/L1Data/RICH1RICH2"] = "Rich::Future::RawBankDecoder/RichFutureDecode"

RawBankToSTClusterAlg("createITClusters").clusterLocation = "Raw/IT/Clusters"
RawBankToSTClusterAlg("createITClusters").DetType = "IT"
RawBankToSTClusterAlg("createITClusters").summaryLocation = "Rec/IT/Summary"
RawBankToSTLiteClusterAlg(
    "createITLiteClusters").clusterLocation = "Raw/IT/LiteClusters"
RawBankToSTLiteClusterAlg("createITLiteClusters").DetType = "IT"

RawBankToSTClusterAlg("createUTClusters").clusterLocation = "Raw/UT/Clusters"
RawBankToSTClusterAlg("createUTClusters").DetType = "UT"
RawBankToSTClusterAlg("createUTClusters").summaryLocation = "Rec/UT/Summary"
RawBankToSTLiteClusterAlg(
    "createUTLiteClusters").clusterLocation = "Raw/UT/LiteClusters"
RawBankToSTLiteClusterAlg("createUTLiteClusters").DetType = "UT"

#Moved to ST/STTools/python/STTools/STOfflineConf, as with TT and IT
#from Configurables import STOfflinePosition
#UT = STOfflinePosition('ToolSvc.UTClusterPosition')
#UT.DetType = "UT"

DecodeVeloRawBuffer("createVeloClusters").DecodeToVeloClusters = True
DecodeVeloRawBuffer("createVeloClusters").DecodeToVeloLiteClusters = False
DecodeVeloRawBuffer("createVeloLiteClusters").DecodeToVeloClusters = False
DecodeVeloRawBuffer("createVeloLiteClusters").DecodeToVeloLiteClusters = True

#Rich__DAQ__RawBufferToRichDigitsAlg("RichRawEventToDigits").DecodeBufferOnly = False
# Calorimeters
from Configurables import CaloDigitConf
CaloDigitConf(EnableDigitsOnDemand=True).applyConf()

importOptions("$L0DUOPTS/L0OnDemand.opts")

print __doc__
