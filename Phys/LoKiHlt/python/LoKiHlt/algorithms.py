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
## @file LoKiHlt/algorithms.py
#  The full set of useful objects from LoKiHlt library
#
#  This file is a part of LoKi project -
#   ``C++ ToolKit  for Smart and Friendly Physics Analysis''
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2008-10-19
#
# =============================================================================
"""
Few useful objects from LoKiHlt library

  This file is a part of LoKi project -
   ``C++ ToolKit  for Smart and Friendly Physics Analysis''

  The package has been designed with the kind help from
  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
  contributions and advices from G.Raven, J.van Tilburg,
  A.Golutvin, P.Koppenburg have been used in the design.

"""
# =============================================================================
__author__ = "Vanya BELYAEV ibelyaev@physics.syr.edu"
__date__ = "2008-10-19"
__version__ = ""
# =============================================================================

from LoKiNumbers.sources import *

ALG_ALLENABLED = LoKi.Algorithms.AllEnabled
ALG_ALLEXECUTED = LoKi.Algorithms.AllExecuted
ALG_ALLPASSED = LoKi.Algorithms.AllPassed

ALG_ANYENABLED = LoKi.Algorithms.AnyEnabled
ALG_ANYEXECUTED = LoKi.Algorithms.AnyExecuted
ALG_ANYPASSED = LoKi.Algorithms.AnyPassed

ALG_ENABLED = LoKi.Algorithms.Enabled
ALG_EXECUTED = LoKi.Algorithms.Executed
ALG_PASSED = LoKi.Algorithms.Passed

ALG_RUN = LoKi.Algorithms.Run
ALG_RUNALL = LoKi.Algorithms.RunAll

ALG_NUMENABLED = LoKi.Algorithms.NumEnabled
ALG_NUMEXECUTED = LoKi.Algorithms.NumExecuted
ALG_NUMPASSED = LoKi.Algorithms.NumPassed

from LoKiNumbers.decorators import decorateVoids as _decorateVoids

_decorated = _decorateVoids(__name__)

from LoKiCore.functions import strings as _strings


# =============================================================================
## construct the 'ALG_ALLENABLED' functor in a nice way
def allEnabled(alg1, *algs):
    """
    Construct the 'ALG_ALLENABLED' predicate in a nice way:

    >>> p2 = allEnabled ( 'alg1' , 'alg2' )
    >>> p3 = allEnabled ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = allEnabled ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = allEnabled ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ALLENABLED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_ALLEXECUTED' functor in a nice way
def allExecuted(alg1, *algs):
    """
    Construct the 'ALG_ALLEXECUTED' predicate in a nice way:

    >>> p2 = allExecuted ( 'alg1' , 'alg2' )
    >>> p3 = allExecuted ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = allExecuted ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = allExecuted ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ALLEXECUTED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_ALLPASSED' functor in a nice way
def allPassed(alg1, *algs):
    """
    Construct the 'ALG_ALLPASSED' predicate in a nice way:

    >>> p2 = allPassed ( 'alg1' , 'alg2' )
    >>> p3 = allPassed ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = allPassed ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = allPassed ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ALLPASSED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_ANYENABLED' functor in a nice way
def anyEnabled(alg1, *algs):
    """
    Construct the 'ALG_ANYENABLED' predicate in a nice way:

    >>> p2 = anyEnabled ( 'alg1' , 'alg2' )
    >>> p3 = anyEnabled ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = anyEnabled ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = anyEnabled ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ANYENABLED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_ANYEXECUTED' functor in a nice way
def anyExecuted(alg1, *algs):
    """
    Construct the 'ALG_ANYEXECUTED' predicate in a nice way:

    >>> p2 = anyExecuted ( 'alg1' , 'alg2' )
    >>> p3 = anyExecuted ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = anyExecuted ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = anyExecuted ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ANYEXECUTED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_ANYPASSED' functor in a nice way
def anyPassed(alg1, *algs):
    """
    Construct the 'ALG_ANYPASSED' predicate in a nice way:

    >>> p2 = anyPassed ( 'alg1' , 'alg2' )
    >>> p3 = anyPassed ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = anyPassed ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = anyPassed ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_ANYPASSED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_NUMENABLED' functor in a nice way
def numEnabled(alg1, *algs):
    """
    Construct the 'ALG_NUMENABLED' functor in a nice way:

    >>> p2 = numEnabled ( 'alg1' , 'alg2' )
    >>> p3 = numEnabled ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = numEnabled ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = numEnabled ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_NUMENABLED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_NUMEXECUTED' functor in a nice way
def numExecuted(alg1, *algs):
    """
    Construct the 'ALG_NUMEXECUTED' functor in a nice way:

    >>> p2 = numExecuted ( 'alg1' , 'alg2' )
    >>> p3 = numExecuted ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = numExecuted ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = numExecuted ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_NUMEXECUTED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_NUMPASSED' functor in a nice way
def numPassed(alg1, *algs):
    """
    Construct the 'ALG_NUMPASSED' functor in a nice way:

    >>> p2 = anyPassed ( 'alg1' , 'alg2' )
    >>> p3 = anyPassed ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = anyPassed ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = anyPassed ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return ALG_NUMPASSED(_strings(alg1, *algs))


# =============================================================================
## construct the 'ALG_RUNALL' functor in a nice way
def allRun(alg1, *algs):
    """
    Construct the 'ALG_RUNALL' functor in a nice way:

    >>> p2 = allRun ( 'alg1' , 'alg2' )
    >>> p3 = allRun ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = allRun ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = allRun ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    _lst = _strings(alg1, *algs)
    if 1 == len(_lst): return ALG_RUN(_lst[0])
    return ALG_RUNALL(_lst)


# =============================================================================
## construct the 'ALG_RUNALL' functor in a nice way
def execute(alg1, *algs):
    """
    Construct the 'ALG_RUNALL' functor in a nice way:
    (alias for allRun-function)

    >>> p2 = execute ( 'alg1' , 'alg2' )
    >>> p3 = execute ( 'alg1' , 'alg2' , 'alg3' )
    >>> p4 = execute ( 'alg1' , 'alg2' , 'alg3' , 'alg4' )
    >>> pl = execute ( [ 'alg1' , 'alg2' , 'alg3', ... ] )

    """
    return allRun(alg1, *algs)


# =============================================================================
if '__main__' == __name__:
    print 'Number of properly decorated types: %s' % len(_decorated)
    for o in _decorated:
        print o

# =============================================================================
# The END
# =============================================================================
