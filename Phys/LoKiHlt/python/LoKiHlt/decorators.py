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
## @file LoKiHlt/decorators.py
#  The set of basic decorator for objects from LoKiHlt library
#
#  This file is a part of LoKi project -
#   'C++ ToolKit  for Smart and Friendly Physics Analysis'
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
# =============================================================================
"""
The set of basic decorators for obejcts from LoKiHlt library

This file is a part of LoKi project -
'C++ ToolKit  for Smart and Friendly Physics Analysis'

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.
"""
# =============================================================================
__author__ = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__version__ = "1.4"
# =============================================================================

from LoKiHlt.functions import *
_name = __name__


# =============================================================================
## make the decoration of all objects fomr this module
def _decorate(name=_name):
    """
    Make the decoration of all objects fomr this module
    """
    import LoKiCore.decorators as _LoKiCore
    _o1 = 'const LHCb::ODIN*'
    _l0 = 'const LHCb::L0DUReport*'
    _hlt = 'const LHCb::HltDecReports*'
    o1 = LHCb.ODIN
    l0 = LHCb.L0DUReport
    hlt = LHCb.HltDecReports

    # "function" : ODIN -> double

    _decorated = _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_o1, 'double'),  ## the base
        LoKi.Dicts.FunCalls(o1),  ## call-traits
        LoKi.Dicts.FuncOps(_o1))  ## operators&operations

    # "function" : L0 -> double

    _decorated |= _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_l0, 'double'),  ## the base
        LoKi.Dicts.FunCalls(l0),  ## call-traits
        LoKi.Dicts.FuncOps(_l0))  ## operators&operations

    # "function" : HLT -> double

    _decorated |= _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_hlt, 'double'),  ## the base
        LoKi.Dicts.FunCalls(hlt),  ## call-traits
        LoKi.Dicts.FuncOps(_hlt))  ## operators&operations

    # "predicate/cut" :  ODIN -> bool

    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_o1, bool),  ## the base
        LoKi.Dicts.CutCalls(o1),  ## call-traits
        LoKi.Dicts.CutsOps(_o1))  ## operators&operations

    # "predicate/cut" :  L0 -> bool

    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_l0, bool),  ## the base
        LoKi.Dicts.CutCalls(l0),  ## call-traits
        LoKi.Dicts.CutsOps(_l0))  ## operators&operations

    # "predicate/cut" :  HLT -> bool
    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_hlt, bool),  ## the base
        LoKi.Dicts.CutCalls(hlt),  ## call-traits
        LoKi.Dicts.CutsOps(_hlt))  ## operators&operations

    ##

    return _decorated  ## RETURN


# =============================================================================
## perform the decoration
_decorated = _decorate()  ## ATTENTION
from LoKiNumbers.decorators import decorateVoids
_decorated |= decorateVoids(__name__)
# =============================================================================

# =============================================================================
if '__main__' == __name__:
    print 'Number of properly decorated types: %s' % len(_decorated)
    for o in _decorated:
        print o

# =============================================================================
# The END
# =============================================================================
