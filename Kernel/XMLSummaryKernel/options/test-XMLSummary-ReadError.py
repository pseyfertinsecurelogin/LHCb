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
# Test, will fail to write a DST because the output mode is wrong
# Will fill some counters, and output the summary.xml
# These options are mostly copied from $GAUDIEXAMPLESROOT/python/CounterEx.py
from Gaudi.Configuration import *
from GaudiConf import IOHelper

importOptions("$XMLSUMMARYKERNELROOT/options/test-XMLSummary.py")

# Set up the Event Selector for a file which does not exist
IOHelper().inputFiles( ['PFN:YouCannotOpenMe.dst'], clear=True)

