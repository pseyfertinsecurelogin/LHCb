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
## @file decorators.py LoKiNumbers/decorators.py
#  The set of basic decorator for objects from LoKiNumberslibrary
#  The file is a part of LoKi and Bender projects
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#
#        This file is a part of LoKi project -
#    "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
# =============================================================================
"""
The set of basic decorators for objects from LoKiNumbers library

    This file is a part of LoKi project -
``C++ ToolKit  for Smart and Friendly Physics Analysis''

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.
"""
# =============================================================================
__author__ = ' Vanya BELYAEV ibelyaev@physics.syr.edu '
__date__ = ' xxxx-xx-xx '
__version__ = ''
# =============================================================================

from LoKiNumbers.functions import *
from LoKiNumbers.sources import *

_name = __name__


# =============================================================================
## make the decoration of all objects fomr this module
def _decorate(name=_name):
    """
    Make the decoration of all objects fomr this module
    """
    import LoKiCore.decorators as _LoKiCore
    _d = 'double'
    _v = 'std::vector<double>'

    # double -> double
    _decorated = _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_d, _d),  ## the base
        LoKi.Dicts.FunCalls(_d),  ## call-traits
        LoKi.Dicts.FuncOps(_d))  ## operators
    # vector<double> -> double
    _decorated |= _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_v, _d),  ## the base
        LoKi.Dicts.FunCalls(_v),  ## call-traits
        LoKi.Dicts.FuncOps(_v))  ## operators
    # double -> bool
    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_d, bool),  ## the base
        LoKi.Dicts.CutCalls(_d),  ## call-traits
        LoKi.Dicts.CutsOps(_d))  ## operators
    # vector<double> -> bool
    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_v, bool),  ## the base
        LoKi.Dicts.CutCalls(_v),  ## call-traits
        LoKi.Dicts.CutsOps(_v))  ## operators
    # vector<double> -> vector<double>
    _decorated |= _LoKiCore.getAndDecorateMaps(
        name,  ## module name
        LoKi.Functor(_v, _v),  ## the base
        LoKi.Dicts.PipeOps(_d))  ## stremers
    # vector<double> -> double
    _decorated |= _LoKiCore.getAndDecorateMaps(
        name,  ## module name
        LoKi.Functor(_v, _d),  ## the base
        LoKi.Dicts.FunValOps(_d))  ## stremers

    # void -> vector<double>
    _decorated |= _LoKiCore.getAndDecorateMaps(
        name,  ## module name
        LoKi.Functor('void', _v),  ## the base
        LoKi.Dicts.SourceOps(_d))  ## stremers

    return _decorated


def decorateVoids(name=_name):
    """
    Make the decoration of 'void'-functors this module
    """
    import LoKiCore.decorators as _LoKiCore
    _d = 'double'
    _b = 'bool'
    _v = 'void'

    # void -> double
    _decorated = _LoKiCore.getAndDecorateFunctions(
        name,  ## module name
        LoKi.Functor(_v, _d),  ## the base
        LoKi.Dicts.FunCalls(_v),  ## call-traits
        LoKi.Dicts.FuncOps(_v))  ## operators

    # void -> bool
    _decorated |= _LoKiCore.getAndDecoratePredicates(
        name,  ## module name
        LoKi.Functor(_v, _b),  ## the base
        LoKi.Dicts.CutCalls(_v),  ## call-traits
        LoKi.Dicts.CutsOps(_v))  ## operators

    return _decorated


# =============================================================================
## perform the decoration
_decorated = _decorate()
_decorated |= decorateVoids()  ## ATTENTION
# =============================================================================

# =============================================================================
if '__main__' == __name__:

    print 80 * '*'
    print __doc__
    print ' Author  : ', __author__
    print ' Version : ', __version__
    print ' Date    : ', __date__
    print 80 * '*'
    for i in _decorated:
        print i

# =============================================================================
# The END
# =============================================================================
