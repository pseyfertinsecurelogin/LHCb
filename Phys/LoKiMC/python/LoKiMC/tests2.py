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
## @file   LoKiMC/tests2.py
#   Test the problem reported in JIRA:
#   @see https://its.cern.ch/jira/browse/LHCBPS-1417
#
#        This file is a part of LoKi project -
#    "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Pieter David pieter.david@nikhef.nl
#  @date 2015-06-01
# =============================================================================
""" Test the problem reported in JIRA:
    https://its.cern.ch/jira/browse/LHCBPS-1417

      This file is a part of LoKi project -
'C++ ToolKit  for Smart and Friendly Physics Analysis'

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.
"""
# =============================================================================
__author__ = "Pieter David pieter.david@nikhef.nl"
__date__ = "2007-05-29"
__version__ = ""
# =============================================================================

from LoKiCore.basic import LoKi, std, cpp
FALL = LoKi.Constant("void", bool)(True)
_c1 = std.vector('const LHCb::MCParticle*')
MCRange = cpp.Gaudi.NamedRange_(_c1)

# =============================================================================
if '__main__' == __name__:

    print 80 * '*'
    print __doc__
    print ' Author  : ', __author__
    print ' Version : ', __version__
    print ' Date    : ', __date__
    print 80 * '*'
    for i in dir():
        print i

# =============================================================================

# =============================================================================
# The END
# =============================================================================
