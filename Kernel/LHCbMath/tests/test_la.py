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
# -*- coding: utf-8 -*-
# =============================================================================
## @file test_la.py
#
#  tests for some linear algebra stuff with ROOT6
#
#  @author Vanya BELYAEV Ivan.Belyaeve@itep.ru
#  @date 2014-09-02
# =============================================================================
"""Tests for some linear algebra studd with ROOT6
"""
# =============================================================================
__version__ = ""
__author__ = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__ = "2014-09-01"
__all__ = ()  ## nothing to be imported
# =============================================================================
import ROOT
import LHCbMath.Types

from LHCbMath.Types import cpp

# ==============================================================================
# Test Lorentz vectors
# ==============================================================================
print 'Operations with Lorentz Vector '
LV = cpp.Gaudi.Math.LorentzVector
a = LV(1, 2, 3, 4)
b = LV(2, 3, 4, 5)
print ' a      ', a
print ' b      ', b
print ' a +  b ', a + b
print ' a -  b ', a - b
print ' a *  b ', a * b
a += b
print ' a += b ', a
a -= b
print ' a -= b ', a
print ' 2 * a  ', 2 * a
print ' a * 2  ', a * 2
print ' a / 2  ', a / 2
print ' a **2  ', a**2

# ==============================================================================
# 3D-point and 3D-vectors
# ==============================================================================
P3 = cpp.Gaudi.XYZPoint
p1 = P3(1, 2, 3)
p2 = P3(4, 5, 6)
#
print 'Operations with 3D-points '
print ' p1 , p2 ', p1, p2
print ' p1 * 2  ', p1 * 2
print ' 2  * p1 ', 2 * p1  ## it fails for earlier verisons
print ' p1 / 2  ', p1 / 2
q = p1 - p2
print ' p1 - p2 ', q, type(q)
p1 *= 2
print ' p1 *=2  ', p1
p1 /= 2
print ' p1 /=2  ', p2

print 'Operations with 3D-vectors '
V3 = cpp.Gaudi.XYZVector
v1 = V3(1, 2, 3)
v2 = V3(4, 5, 6)

print ' v1 , v2', v1, v2
print ' v1 + v2', v1 + v2
print ' v1 - v2', v1 - v2
print ' v1 * 2  ', v1 * 2
print ' 2  * v1 ', 2 * v1
print ' v1 / 2  ', v1 / 2
v1 *= 2
print ' v1 *=2  ', v1
v1 /= 2
print ' v1 /=2  ', v2
print ' v1 * v2 ', v1 * v2
print ' v1**2   ', v1**2

print 'Operations with 3D-vectors and points '

print 'p1+v1', p1 + v1, type(p1 + v1)
print 'p1-v1', p1 - v1, type(p1 - v1)
print 'v1+p1', v1 + p1, type(v1 + p1)

# ============================================================================
# The END
# =============================================================================
