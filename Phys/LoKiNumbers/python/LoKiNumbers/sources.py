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
# $Id$
# =============================================================================
## @file LoKiNumbers/sources.py
#  The full set of useful objects from LoKiNumbers library
#  The file is a part of LoKi and Bender projects
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
# =============================================================================
"""
Few useful objects from LoKiNumbers library
"""
__author__ = "Vanya BELYAEV ibelyaev@physics.syr.edu"
__version__ = "CVS Tag $Name: not supported by cvs2svn $, version $Revision$ "
# =============================================================================

import LoKiCore.decorators as _LoKiCore

# Namespaces:
cpp = _LoKiCore.cpp
std = _LoKiCore.std
LoKi = _LoKiCore.LoKi
# =============================================================================

_d = 'double'
_v = 'void'
_b = bool

FCuts = LoKi.Functor(_v, bool)
FCut = LoKi.FunctorFromFunctor(_v, bool)
FFunc = LoKi.Functor(_v, _d)
FFun = LoKi.FunctorFromFunctor(_v, _d)

FALL = LoKi.Constant(_v, bool)(True)
FNONE = LoKi.Constant(_v, bool)(False)

FONE = LoKi.Constant(_v, _d)(1.0)
FNULL = LoKi.Constant(_v, _d)(0.0)
FZERO = LoKi.Constant(_v, _d)(0.0)

## @see LoKi::Cuts::FRATE
RATE = LoKi.Scalers.RateLimitV
## @see LoKi::Cuts::FSCALE
SCALE = LoKi.Scalers.RandomScaleV
## @see LoKi::Cuts::FSKIP
SKIP = LoKi.Scalers.SkipperV

# =============================================================================
# The END
# =============================================================================
