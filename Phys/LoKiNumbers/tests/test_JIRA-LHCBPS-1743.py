#!/usr/bin/env python
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
# =============================================================================
## @file
#  The test for JIRA LHCBPS-1743
#  @see  https://its.cern.ch/jira/browse/LHCBPS-1743
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2007-05-29
# =============================================================================
""" The test for JIRA LHCBPS-1743
- https://its.cern.ch/jira/browse/LHCBPS-1743
"""
__author__ = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
# =============================================================================

from LoKiNumbers.decorators import *
from LoKiCore.functions import *

first = 0 > FONE

second = monitor(FONE > 0, '"  -there should be no such prints  here!\n',
                 'ERROR: "')

func = first & second

print 'BEFORE'
result = func()  ## If  everything is fine it must be silent!
print 'AFTER'

# =============================================================================
# The END
# =============================================================================
