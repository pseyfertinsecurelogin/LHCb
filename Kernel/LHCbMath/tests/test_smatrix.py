#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# $Id$ 
# =============================================================================
## @file test_la.py
#
#  tests for some linear algebra stuff with ROOT6
#
#  @author Vanya BELYAEV Ivan.Belyaeve@itep.ru
#  @date 2014-09-02
# 
#                    $Revision$
#  Last modification $Date$
#                 by $Author$
# =============================================================================
"""Tests for some linear algebra studd with ROOT6
"""
# =============================================================================
__version__ = "$Revision:"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2014-09-01"
__all__     = ()  ## nothing to be imported 
# =============================================================================
import ROOT
import LHCbMath.Types 

from   LHCbMath.Types import cpp 
# ============================================================================

print 'TEST vectors: '

LA3 = cpp.Gaudi.Math.Vector3
l1  = LA3(0,1,2)
l2  = LA3(3,4,5)

print 'l1 , l2 '   , l1 , l2 
print 'l1 + l2 '   , l1 + l2
print 'l1 - l2 '   , l1 - l2
print 'l1 * l2 '   , l1 * l2
print 'l1 *  2 '   , l1 *  2
print ' 2 + l1 '   , 2  + l1
print ' 2 - l1 '   , 2  - l1
print 'l1 +  2 '   , l1 +  2
print ' 2 * l1 '   , 2  * l1
print 'l1 /  2 '   , l1 /  2
l1 *= 2
print 'l1 *=  2 '  , l1 
l1 /= 2
print 'l1 /=  2 '  , l1 
l1 += 2
print 'l1 +=  2 '  , l1 
l1 -= 2
print 'l1 -=  2 '  , l1 


print 'TEST matrices '

m22 = cpp.Gaudi.Math.Matrix(2,2) ()
m23 = cpp.Gaudi.Math.Matrix(2,3) ()
s22 = cpp.Gaudi.Math.SymMatrix(2)()

l2  = cpp.Gaudi.Math.Vector(2)()
l3  = cpp.Gaudi.Math.Vector(3)()

l2[0]    = 1
l2[1]    = 2

l3[0]    = 1
l3[1]    = 2
l3[1]    = 3

print 'l2 , l3 '   , l2 , l3

m22[0,0] = 1
m22[0,1] = 1
m22[1,1] = 1

m23[0,0] = 1
m23[1,1] = 1
m23[0,2] = 1

s22[0,0] = 2
s22[1,0] = 1
s22[1,1] = 3

print 'm22\n%s'   % m22
print 's22\n%s'   % s22
print 'm23\n%s'   % m23
print 'm22/3\n%s' % (m22/3)
print 'm23*3\n%s' % (m23*3)


print 'm22*m23:\n%s' % ( m22 * m23 )
print 'm22*l2:' ,        m22 * l2  
print 'l2*m22:' ,        l2  * m22 
print 'm23*l3:' ,        m23 * l3  
print 'l2*m23:' ,         l2 * m23 

print m22

print 'm22*s22+2*m22:\n' , m22*s22 + 2*m22 


# ============================================================================
# The END 
# =============================================================================
