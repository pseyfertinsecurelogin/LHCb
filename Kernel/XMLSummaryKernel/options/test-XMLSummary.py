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
# Test, will read and write a Dst file
# Will fill some counters, and output the summary.xml
# These options are mostly copied from $IOEXAMPLEROOT/tests/options/WriteDst.opts
from Gaudi.Configuration import *

from Configurables import EventCountAlg, LHCbApp
from Configurables import XMLSummarySvc
from XMLSummaryKernel.Configuration import XMLSummary
#from Configurables import LHCbApp

#---------------------------------------------------------------------------
# Setup read and write of a ROOT dst file
#---------------------------------------------------------------------------
ApplicationMgr().HistogramPersistency = "NONE"
ApplicationMgr().EvtMax = 5
EventSelector().PrintFreq = 1

# Set up persistency
LHCbApp()

#Define the input file and output file to copy
importOptions("$XMLSUMMARYKERNELROOT/options/RootDst.py")

#---------------------------------------------------------------------------
# Add a counting algorithm
#---------------------------------------------------------------------------

EvtAlg = EventCountAlg("TestCounter")

ApplicationMgr().TopAlg += [EvtAlg]

#---------------------------------------------------------------------------
# Setup XMLSummarySvc
#---------------------------------------------------------------------------

xmlsummary = XMLSummarySvc("CounterSummarySvc")
xmlsummary.OutputLevel = 1
xmlsummary.UpdateFreq = 1

XMLSummary().XMLSummary = "summary.xml"
