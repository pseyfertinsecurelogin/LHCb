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
from Gaudi.Configuration import *

# Job options to configure the data on demand service for decoding the RawEvent
print "# WARNING: You have imported DecodeRawEvent.py, this is now replaced with a configurable. See savannah task #19106."

from Configurables import DecodeRawEvent
DecodeRawEvent().DataOnDemand=True

importOptions( "$L0TCK/L0DUConfig.opts" )
