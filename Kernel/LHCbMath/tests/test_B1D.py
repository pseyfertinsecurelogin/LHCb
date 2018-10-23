#!/usr/bin/env python
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
# -*- coding: utf-8 -*-
# =============================================================================
## @file  test_B1D.py
#  Test utilities for 1D Bernstein Polynomials 
# =============================================================================
import ROOT, random, math  

def check_equality ( a ,  b , message = '' , tolerance = 1.e-7 ) :
    d = abs ( a - b ) 
    if d > tolerance :
        raise ValueError ( message + ' |%s-%s|=%s>%s' % ( a , b , d , tolerance ) )
    
BP = ROOT.Gaudi.Math.Bernstein
VD = ROOT.std.vector  ('double')
CD = ROOT.std.complex ('double')
VC = ROOT.std.vector  ( CD )

# 1) create & evaluate the polynom 

b = BP ( 5 , 0. , 2. ) ## 5th order for x in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,1)

for i in  range(100) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = b(x)
    check_equality ( y , 1 , 'Invalid polynom value:' )

print 'Check for constant polynomial         is OK'

# 2) create & evaluate the polynom

for i in  range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )
ymin = min ( b.pars() )
ymax = max ( b.pars() )

for i in  range(100) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = b(x)
    if not ymin <= y <= ymax : 
        raise ValueError ( 'Invalid polynom value y(%s)=%s (%s/%s)' % ( x , y , ymin , ymax ) )

print 'Check for random polynomial           is OK'

# 3) construct Berstein interpolant
xv = VD ()
yv = VD () 
for x  in  ( 0.0 , 0.1 , 0.2 , 0.3 ,   0.6 , 0.7 , 0.9 , 0.95 , 1.0  ) :
    xv.push_back ( x              ) 
    yv.push_back ( math.sin ( x ) )
    
b = BP ( xv ,  yv , 0 , 1 )
for i in  range (100) :
    x  = random.uniform ( b.xmin() , b.xmax() )
    y  = b(x)
    yt = math.sin ( x )
    check_equality ( y , yt , 'Invalid polynom interpolation' , 1.e-5 )

print 'Check for Bernstein interpolant       is OK'

# 4) construct Berstein polynomials from roots 
xv = VD()
for x in (0.5,1.0,1.5,2,2.5) : xv.push_back ( x ) 
b = BP ( 0 , 3 , xv )
for x in xv :
    y = b(x)
    check_equality ( y , 0 , 'Invalid root[1]' ,  1.e-10 ) 

for i in range (30) :
    x  = random.uniform ( b.xmin() , b.xmax() )
    if x in xv :  continue 
    y  = b(x)
    if abs(y) < 1.e-10 : 
        raise ValueError ( 'Invalid root[2] %s/%s' % (x,y) )
    

print 'Check for Bernstein roots             is OK'

# 5) Check for integrals & derivatives 

b  = BP ( 5 , 0. , 2. ) ## 5th order for x in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,1)
bi = b .indefinite_integral(0)
d  = bi.derivative() 

def ff1 ( x , pars = [] ) :
    return b( x[0] )

f1 = ROOT.TF1( 'f1' , ff1 , b.xmin() , b.xmax() )

for i in range (10) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( b.xmin() , b.xmax() )
    i0 = x2 - x1
    i  = b.integral ( x1 , x2 )
    check_equality ( i     , i0                , 'Invalid integral[1]'    , 1.e-6 )
    ii = bi(x2)-bi(x1) 
    check_equality ( ii    , i0                , 'Invalid integral[2]'    , 1.e-6 )
    ##fi = f1.Integral( x1 , x2 ) 
    ##check_equality ( i     , fi                , 'Invalid integral[3]'    , 1.e-6 )
    
    check_equality ( b(x1) , d(x1)             , 'Invalid derivative [1]' , 1.e-6 )
    check_equality ( b(x1) , bi.derivative(x1) , 'Invalid derivative [2]' , 1.e-6 )

i  = b.integral()
ii = b.integral( b.xmin()+1.e-7 , b.xmax()-1.e-7 )
check_equality ( i , ii , 'Invalid integral[4]' , 1.e-6 ) 
    
del f1, ff1 

print 'Check for Bernstein integrals         is OK'


# 6) Check for elevate/reduce 
for i in  range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

for r in (1,2,3,4,5) : 
    be = b.elevate(r)
    br = be.reduce(r)
    for i in range(50) :
        x = random.uniform ( b.xmin() , b.xmax() )
        y  = b(x)
        ye = be(x)
        yr = br(x)
        check_equality ( y  , ye  , 'Invalid elevate' , 1.e-6 )
        check_equality ( y  , yr  , 'Invalid reduce'  , 1.e-6 )
        
print 'Check for Bernstein elevate/reduce    is OK'
    
# 6)  check  for divisions
r1 = VD()
for x in (0,0,0) : r1.push_back ( x )
r2 = VD()
for x in (0,)    : r2.push_back ( x )
b1 = BP ( 0 , 3 , r1 )
b2 = BP ( 0 , 3 , r2 )
r = b1.divmod(b2)
q = r.first
r = r.second
check_equality ( q.par(0) , 0 , 'Invalid quotient[0]' )
check_equality ( q.par(1) , 0 , 'Invalid quotient[1]' )
check_equality ( r.par(0) , 0 , 'Invalid reminder'    )

print 'Check for Bernstein quotient/reminder is OK'


## 6) check for BernsteinEven
BE = ROOT.Gaudi.Math.BernsteinEven

b  = BE( 5 , 0 , 2 )
for i in range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

xmid = 0.5 * ( b.xmax() + b.xmin() ) 
for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    dx = x1   - xmid  
    x2 = xmid - dx 
    y1 = b(x1)
    y2 = b(x2)
    check_equality ( y1 , y2  , 'Invalid BernsteinEven'  , 1.e-7 )
    
    
print 'Check for BernsteinEven               is OK'
    

## 7)  check for Positive
BP = ROOT.Gaudi.Math.Positive

b  = BP ( 5 , 0 , 3 )
c  = 1.0 / ( b.xmax() - b.xmin() ) 
for i in  range(100) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = b(x)
    check_equality ( y , c , 'Invalid positive polynom value:' )

print 'Check for Positive constant           is OK'

for i in range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

for i in range(500) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = b(x)
    if y <= 0 :
        raise ValueError ( 'Invalid Positive y(%s)=%s' % ( x , y ) )

print 'Check for Positive                    is OK'

## 8) check for PositiveEven 

BPE = ROOT.Gaudi.Math.PositiveEven

b = BPE ( 4 , 0 , 3 )
for i in range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

xmid = 0.5 * ( b.xmax() + b.xmin() ) 
for i in range(500) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = b(x)
    if y <= 0 :
        raise ValueError ( 'Invalid PositiveEven y(%s)=%s' % ( x , y ) )
    x1 = x 
    dx = x1   - xmid  
    x2 = xmid - dx 
    y1 = b(x1)
    y2 = b(x2)
    check_equality ( y1 , y2  , 'Invalid PositiveEven'  , 1.e-7 )
    

print 'Check for PositiveEven                is OK'


## 8-9) check for Monothonic 
BPM = ROOT.Gaudi.Math.Monothonic

b = BPM ( 5 , 0 , 2 , True )
for i in range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

for i in range(500) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    y1 = b(x1)
    if y1 <= 0 :
        raise ValueError ( 'Invalid Increasing y(%s)=%s' % ( x1 , y1 ) )
    x2 = random.uniform ( x1 , b.xmax() )
    y2 = b(x2)
    if y2 <= 0 :
        raise ValueError ( 'Invalid Increasing y(%s)=%s' % ( x2 , y2 ) )
    if y1 > y2 :
        raise ValueError ( 'Invalid Increasing y(%s)=%s>y(%s)=%s' % ( x1 , y1 , x2 , y1 ) )
    

b = BPM ( 5 , 0 , 2 , False )
for i in range ( b.npars() ) : b.setPar ( i , random.uniform ( -10 , 10 ) )

for i in range(500) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    y1 = b(x1)
    if y1 <= 0 :
        raise ValueError ( 'Invalid Decreasing y(%s)=%s' % ( x1 , y1 ) )
    x2 = random.uniform ( x1 , b.xmax() )
    y2 = b(x2)
    if y2 <= 0 :
        raise ValueError ( 'Invalid Decreasing y(%s)=%s' % ( x2 , y2 ) )
    if y1 < y2 :
        raise ValueError ( 'Invalid Decreasing y(%s)=%s<y(%s)=%s' % ( x1 , y1 , x2 , y2 ) )
    
print 'Check for Monothonic(x2)              is OK'

## 10-13) check for Convex
BPC = ROOT.Gaudi.Math.Convex

b_11 = BPC ( 5 , 0 , 2 , True  , True  )
b_01 = BPC ( 5 , 0 , 2 , False , True  )
b_10 = BPC ( 5 , 0 , 2 , True  , False )
b_00 = BPC ( 5 , 0 , 2 , False , False )

for b in ( b_11   , b_01 , b_10 , b_00 ) : 
    for i in range ( b.npars() ) :
        b.setPar ( i , random.uniform ( -10 , 10 ) )

for i in range(500) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )

    for b in ( b_11   , b_01 , b_10 , b_00 ) :
        for x in  ( x1 , x2 ) :
            y = b(x) 
            if y < 0 :
                raise ValueError ( 'Invalid Convex y(%s)=%s' % ( x , y ) )

    ##  b_11 
    y1 = b_11 ( x1 )
    y2 = b_11 ( x2 )
    ym = b_11 ( 0.5*(x1+x2) ) 

    if y1 > y2 :
        raise ValueError ( 'Invalid Convex(11)[0] y(%s)=%s>y(%s)=%s' % ( x1 , y1 , x2 , y2 ) )
    
    if 0.5 * ( y1 + y2 ) < ym :
        raise ValueError ( 'Invalid Convex(11)[2] ' )

    ##  b_01 
    y1 = b_01 ( x1 )
    y2 = b_01 ( x2 )
    ym = b_01 ( 0.5*(x1+x2) ) 

    if y1 < y2 :
        raise ValueError ( 'Invalid Convex(01)[0] y(%s)=%s<y(%s)=%s' % ( x1 , y1 , x2 , y2 ) )
    
    if 0.5 * ( y1 + y2 ) < ym :
        raise ValueError ( 'Invalid Convex(01)[2] ' )
    
    ##  b_10 
    y1 = b_10 ( x1 )
    y2 = b_10 ( x2 )
    ym = b_10 ( 0.5*(x1+x2) ) 

    if y1 > y2 :
        raise ValueError ( 'Invalid Convex(10)[0] y(%s)=%s>y(%s)=%s' % ( x1 , y1 , x2 , y2 ) )
    
    if 0.5 * ( y1 + y2 ) > ym :
        raise ValueError ( 'Invalid Convex(10)[2] ' )

    ##  b_00 
    y1 = b_00 ( x1 )
    y2 = b_00 ( x2 )
    ym = b_00 ( 0.5*(x1+x2) ) 

    if y1 < y2 :
        raise ValueError ( 'Invalid Convex(00)[0] y(%s)=%s<y(%s)=%s' % ( x1 , y1 , x2 , y2 ) )
    
    if 0.5 * ( y1 + y2 ) > ym :
        raise ValueError ( 'Invalid Convex(00)[2] ' )
    

print 'Check for Convex(x4)                  is OK'
    
    
## 10-13) check for ConvexOnly
BPCO = ROOT.Gaudi.Math.ConvexOnly

b_1 = BPCO ( 5 , 0 , 2 , True  )
b_0 = BPCO ( 5 , 0 , 2 , False )
    
for b in ( b_1 , b_0 ) : 
    for i in range ( b.npars() ) :
        b.setPar ( i , random.uniform ( -10 , 10 ) )
    
    
for i in range(500) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )

    for b in ( b_1 , b_0 ) :
        for x in  ( x1 , x2 ) :
            y = b(x) 
            if y < 0 :
                raise ValueError ( 'Invalid ConvexOnly y(%s)=%s' % ( x , y ) )

    ##  b_1 
    y1 = b_1 ( x1 )
    y2 = b_1 ( x2 )
    ym = b_1 ( 0.5*(x1+x2) ) 

    if 0.5 * ( y1 + y2 ) < ym :
        raise ValueError ( 'Invalid ConvexOnly(1)[2] ' )

    ##  b_0 
    y1 = b_0 ( x1 )
    y2 = b_0 ( x2 )
    ym = b_0 ( 0.5*(x1+x2) ) 
    
    if 0.5 * ( y1 + y2 ) > ym :
        raise ValueError ( 'Invalid ConvexOnly(0)[2] ' )
    

print 'Check for ConvexOnly(x2)              is OK'

# =============================================================================
# The END 
# =============================================================================
