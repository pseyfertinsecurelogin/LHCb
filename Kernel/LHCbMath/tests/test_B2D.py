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
## @file  test_B2D.py
#  Test utilities for 2D Bernstein Polynomials 
# =============================================================================
import ROOT, random, math  

def check_equality ( a ,  b , message = '' , tolerance = 1.e-7 ) :
    d = abs ( a - b ) 
    if d > tolerance :
        raise ValueError ( message + ' |%s-%s|=%s>%s' % ( a , b , d , tolerance ) )

BP2D = ROOT.Gaudi.Math.Bernstein2D
VD   = ROOT.std.vector  ('double')

# 1) create & evaluate the polynom 

b = BP2D ( 5 , 5 , 0. , 2. , 0 , 2 ) ## 5x2th order for x,y in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,1)

sc = (b.xmax()-b.xmin())*(b.ymax()-b.ymin())/((b.nX()+1)*(b.nY()+1))
for i in  range(100) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = b(x,y)
    check_equality ( z , 1/sc , 'Invalid polynom value' )

print 'Check for constant 2D-polynomial      is OK'

# 2) create & evaluate  symmetric 
BP2DS = ROOT.Gaudi.Math.Bernstein2DSym

b = BP2DS ( 2 , 0. , 2. ) ## 2x2th order for x,y in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,1)

sc = (b.xmax()-b.xmin())*(b.ymax()-b.ymin())/((b.nX()+1)*(b.nY()+1))
for i in  range(100) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = b(x,y)
    check_equality ( z , 1/sc , 'Invalid polynom value' )

for i in  range ( b.npars() ) : b.setPar(i, random.uniform(-10,10) )

for i in  range(100) :
    x  = random.uniform ( b.xmin() , b.xmax() )
    y  = random.uniform ( b.ymin() , b.ymax() )
    z1 = b(x,y)
    z2 = b(y,x)
    check_equality ( z1 , z2 , 'Invalid symmetric polynom' )

print 'Check for symmetric 2D-polynomial(x2) is OK'

# 3) check integrals

b = BP2D ( 2 , 2 , 0. ,  1 , 0 , 2 ) ## 2x2th order for x,y in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,   random.uniform(-1,1) )

tiny = 1.e-7
i  = b.integral()
ii = b.integral( b.xmin() + tiny , b.xmax() - tiny ,
                 b.ymin() + tiny , b.ymax() - tiny ) ;
check_equality ( i , ii , 'Invalid integrals[0]' , 1.e-4 ) 

print 'Check for integrals[0]                is OK'

def _f1 ( x , pars = [] ) :
    return b( x[0] , x[1] )
f1 = ROOT.TF2('f1',_f1, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )
fi = f1.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )
check_equality ( i , fi , 'Invalid integrals[1]' , 1.e-4 ) 

print 'Check for integrals[1]                is OK'

xmid = 0.5*(b.xmin()+b.xmax())
ymid = 0.5*(b.ymin()+b.ymax())

for i in  range(20) :
    x1  = random.uniform ( b.xmin() ,   xmid   )
    x2  = random.uniform (   xmid   , b.xmax() )
    y1  = random.uniform ( b.ymin() ,   ymid   )
    y2  = random.uniform (   ymid   , b.ymax() )
    z1  =  b (x1,y1)
    z2  =  b (x2,y2)
    v1  =  f1(x1,y1)
    v2  =  f1(x2,y2)
    check_equality ( z1 , v1 , 'Invalid values' , 1.e-6 ) 
    check_equality ( z2 , v2 , 'Invalid values' , 1.e-6 )     
    i   = b.integral  ( x1 , x2 ,  y1 ,  y2 )
    fi  = f1.Integral ( x1 , x2 ,  y1 ,  y2 )
    check_equality ( i , fi , 'Invalid integrals[2]' , 1.e-4 ) 
    
del f1, _f1 

print 'Check for integrals[2]                is OK'


b = BP2DS ( 2 , 0. , 2. ) ## 5x2th order for x,y in [0,2] 
for i in  range ( b.npars() ) : b.setPar(i,   random.uniform(-1,1) )

i  = b.integral() 
ii = b.integral( b.xmin() + tiny , b.xmax() - tiny ,
                 b.ymin() + tiny , b.ymax() - tiny ) ;
check_equality ( i , ii , 'Invalid integrals[3]' , 1.e-4 ) 

def _f2 ( x , pars = [] ) :
    return b( x[0] , x[1] )
f2 = ROOT.TF2('f2',_f2, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )
fi = f2.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )
check_equality ( i , fi , 'Invalid integrals[3]' , 1.e-4 ) 

print 'Check for integrals[3]                is OK'

for i in  range(10) :
    x1  = random.uniform ( b.xmin() , b.xmax() )
    x2  = random.uniform ( x1       , b.xmax() )
    y1  = random.uniform ( b.ymin() , b.ymax() )
    y2  = random.uniform ( y1       , b.ymax() )
    z1  =  b (x1,y1)
    z2  =  b (x2,y2)
    v1  =  f2(x1,y1)
    v2  =  f2(x2,y2)
    check_equality ( z1 , b(y1,x1) , 'Invalid values' , 1.e-6 ) 
    check_equality ( z2 , b(y2,x2) , 'Invalid values' , 1.e-6 ) 
    check_equality ( z1 , v1       , 'Invalid values' , 1.e-6 ) 
    check_equality ( z2 , v2       , 'Invalid values' , 1.e-6 )     
    i   = b.integral  ( x1 , x2 ,  y1 ,  y2 )
    fi  = f2.Integral ( x1 , x2 ,  y1 ,  y2 )
    check_equality ( i , fi , 'Invalid integrals[4]' , 1.e-4 ) 
    
del f2, _f2     
print 'Check for integrals[4]                is OK'

## positive polynomials 

BP2DP = ROOT.Gaudi.Math.Positive2D
b     = BP2DP ( 2 , 2 ,  0 , 2 , 0 , 2 ) 

for i in  range(100) :    
    x1  = random.uniform ( b.xmin() , b.xmax() )
    y1  = random.uniform ( b.ymin() , b.ymax() )
    x2  = random.uniform ( x1       , b.xmax() )
    y2  = random.uniform ( y1       , b.ymax() )
    z1 = b(x1,y1)
    z2 = b(x2,y2)
    check_equality ( z1, z2 , 'Invalid values[2]' , 1.e-4 ) 
    

for i in  range ( b.npars() ) : b.setPar(i,   random.uniform(-10,10) )
def _f3 ( x , pars = [] ) :
    return b( x[0] , x[1] )
f3 = ROOT.TF2('f3',_f3, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )

for i in  range(100) :    
    x1  = random.uniform ( b.xmin() , b.xmax() )
    y1  = random.uniform ( b.ymin() , b.ymax() )
    x2  = random.uniform ( x1       , b.xmax() )
    y2  = random.uniform ( y1       , b.ymax() )
    
    i  = b .integral ( x1 , x2 ,  y1  ,  y2 )
    fi = f3.Integral ( x1 , x2 ,  y1  ,  y2 )
    
    check_equality ( i, fi , 'Invalid integrals[5]' , 1.e-4 ) 
    
del f3,_f3    
print 'Check for integrals[5]                is OK'


## positive symmetric polynomials 

BP2DPS = ROOT.Gaudi.Math.Positive2DSym
b     = BP2DPS ( 2 , 0 , 2 ) 

for i in  range(100) :    
    x1  = random.uniform ( b.xmin() , b.xmax() )
    y1  = random.uniform ( b.ymin() , b.ymax() )
    x2  = random.uniform ( x1       , b.xmax() )
    y2  = random.uniform ( y1       , b.ymax() )
    z1 = b(x1,y1)
    z2 = b(x2,y2)
    check_equality ( z1, z2       , 'Invalid values[2]' , 1.e-4 ) 
    check_equality ( z1, b(y1,x1) , 'Invalid values[2]' , 1.e-4 ) 
    check_equality ( z2, b(y2,x2) , 'Invalid values[2]' , 1.e-4 ) 

for i in  range ( b.npars() ) : b.setPar(i,   random.uniform(-10,10) )

i  = b.integral() 
ii = b.integral( b.xmin() + tiny , b.xmax() - tiny ,
                 b.ymin() + tiny , b.ymax() - tiny ) ;
check_equality ( i , ii , 'Invalid integrals[6]' , 1.e-4 ) 


def _f4 ( x , pars = [] ) :
    return b( x[0] , x[1] )
f4 = ROOT.TF2('f4',_f4, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )

i  = b.integral() 
ii = b.integral( b.xmin() + tiny , b.xmax() - tiny ,
                 b.ymin() + tiny , b.ymax() - tiny ) ;
fi = f4.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() )
check_equality ( i , fi , 'Invalid integrals[7]' , 1.e-4 ) 

xmid = 0.5*(b.xmin()+b.xmax())
ymid = 0.5*(b.ymin()+b.ymax())
for i in  range(100) :    
    x1  = random.uniform ( b.xmin() ,   xmid   )
    y1  = random.uniform ( b.ymin() ,   ymid   )
    x2  = random.uniform (   xmid   , b.xmax() )
    y2  = random.uniform (   ymid   , b.ymax() )
    
    i  = b .integral ( x1 , x2 ,  y1  ,  y2 )
    fi = f4.Integral ( x1 , x2 ,  y1  ,  y2 )
    
    check_equality ( i, fi , 'Invalid integrals[8]' , 1.e-4 ) 
    
del f4,_f4    
print 'Check for integrals[6]                is OK'
