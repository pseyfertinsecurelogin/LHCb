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
## @file decorators.py LoKiCore/basic.py
#
#  The set of basic decorator for objects from LoKiCore library
#
#        This file is a part of LoKi project -
#    "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#
# =============================================================================
"""
The set of basic decorators for objects from LoKiCore library

    This file is a part of LoKi project -
``C++ ToolKit  for Smart and Friendly Physics Analysis''

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.

"""
# =============================================================================
__author__ = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__date__ = "2010-09-27"
__version__ = ""
# =============================================================================
__all__ = (
    'cpp',  ## global C++ namespace
    'std',  ## C++ 'std'   namespace
    'LoKi',  ## C++ 'LoKi'  namespace
    'LHCb',  ## C++ 'LHCb'  namespace
    'Gaudi'  ## C++ 'Gaudi' namespace
)
# =============================================================================
import cppyy

# namespaces shortcuts
cpp = cppyy.gbl
std = cpp.std
LoKi = cpp.LoKi
LHCb = cpp.LHCb
Gaudi = cpp.Gaudi

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
# The END
# =============================================================================
