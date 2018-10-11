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
## @file  test_B3D.py
#  Test utilities for 3D Bernstein Polynomials 
# =============================================================================
import ROOT, random, math  

def check_equality ( a ,  b , message = '' , tolerance = 1.e-7 ) :
    d = abs ( a - b ) 
    if d > tolerance :
        raise ValueError ( message + ' |%s-%s|=%s>%s' % ( a , b , d , tolerance ) )
    
BP = ROOT.Gaudi.Math.Bernstein3D
VD = ROOT.std.vector  ('double')


# 1) create & evaluate the polynom 

b = BP ( 3 , 4 , 2  , 0. , 1. , 0  , 2 , 0  , 3 )
for i in  range ( b.npars() ) : b.setPar(i,1)

sc  = ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
sc /= ( b.nX () + 1 )
sc /= ( b.nY () + 1 )
sc /= ( b.nZ () + 1 )

for i in  range(10) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for constant polynomial         is OK'

# 2) check integrals

b = BP ( 3 , 4, 5 , 0. , 1. , 0. , 2. , 0, 3 )  
for i in  range ( b.npars() ) : b.setPar(i,   random.uniform(-1,1) )

tiny = 1.e-7
i  = b.integral()
ii = b.integral( b.xmin() + tiny , b.xmax() - tiny ,
                 b.ymin() + tiny , b.ymax() - tiny ,
                 b.zmin() + tiny , b.zmax() - tiny )
                 
check_equality ( i , ii , 'Invalid integrals[0]' , 1.e-4 ) 

print 'Check for integrals[0]                is OK'

def _f1 ( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f1 = ROOT.TF3('f1',_f1, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f1.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )

check_equality ( i , fi , 'Invalid integrals[1]' , 1.e-4 ) 

print 'Check for integrals[1]                is OK'


for i in range(100) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )

    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f1.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[2]' , 1.e-4 ) 

del f1,_f1 
print 'Check for integrals[2]                is OK'


# 2) create & evaluate  symmetric 

BPS = ROOT.Gaudi.Math.Bernstein3DSym
b   = BPS ( 2  , 0. , 2  )
for i in  range ( b.npars() ) : b.setPar(i,1)


sc  = ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
sc /= ( b.nX () + 1 )
sc /= ( b.nY () + 1 )
sc /= ( b.nZ () + 1 )
 
for i in  range(50) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for symmetrix polynomial        is OK'
    
for i in  range ( b.npars() ) : b.setPar(i, random.uniform(-1,1) )
    

def _f2 ( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f2 = ROOT.TF3('f2',_f2, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f2.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )
i  = b.integral() 
check_equality ( i , fi , 'Invalid integrals[3]' , 1.e-4 ) 

print 'Check for integrals[3]                is OK'


for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )
    
    check_equality ( b ( x1 , y1 , z1  ) , b ( y1 , x1 , z1  ) , "Invalid  value(1)" )
    check_equality ( b ( x1 , y1 , z1  ) , b ( x1 , z1 , y1  ) , "Invalid  value(2)" )
    check_equality ( b ( x1 , y1 , z1  ) , b ( z1 , y1 , x1  ) , "Invalid  value(3)" )
        
    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f2.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[4]' , 1.e-4 ) 

del f2,_f2
print 'Check for integrals[4]                is OK'


# 2) create & evaluate  mixed poly

BPM = ROOT.Gaudi.Math.Bernstein3DMix
b   = BPM ( 3  , 2 , 0. , 2  , 0 , 3 )  
for i in  range ( b.npars() ) : b.setPar(i,1)


sc  = ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
sc /= ( b.nX () + 1 )
sc /= ( b.nY () + 1 )
sc /= ( b.nZ () + 1 )
 
for i in  range(50) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for mixed polynomial            is OK'
    
for i in  range ( b.npars() ) : b.setPar(i,random.uniform(-2,2) )

def _f3( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f3 = ROOT.TF3('f3',_f3, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f3.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )
i  = b.integral() 
check_equality ( i , fi , 'Invalid integrals[5]' , 1.e-4 ) 

print 'Check for integrals[5]                is OK'


for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )
    
    check_equality ( b ( x1 , y1 , z1  ) , b ( y1 , x1 , z1  ) , "Invalid  value(1)" )
    
    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f3.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[6]' , 1.e-4 ) 

del f3,_f3
print 'Check for integrals[6]                is OK'



# 2) create & evaluate positive 

BPP = ROOT.Gaudi.Math.Positive3D
b   = BPP ( 3  , 2 , 4 , 0. , 2  , 0 , 3 , 0 , 5 )

sc  = 1.0 
sc *= ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
 
for i in  range(50) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for positive polynomial         is OK'
    

for i in  range ( b.npars() ) : b.setPar(i,random.uniform(-2,2) )

def _f4( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f4 = ROOT.TF3('f4',_f4, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f4.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )
i  = b.integral() 
check_equality ( i , fi , 'Invalid integrals[7]' , 1.e-4 ) 

print 'Check for integrals[7]                is OK'

for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )

    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f4.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[8]' , 1.e-4 ) 

del f4,_f4
print 'Check for integrals[8]                is OK'



# 2) create & evaluate positive 

BPPS = ROOT.Gaudi.Math.Positive3DSym
b    = BPPS ( 3  , 0 , 5 )

sc  = 1.0 
sc *= ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
 
for i in  range(50) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for positive symmetric          is OK'
    

for i in  range ( b.npars() ) : b.setPar(i,random.uniform(-2,2) )


def _f5( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f5 = ROOT.TF3('f5',_f5, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f5.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )
i  = b.integral() 
check_equality ( i , fi , 'Invalid integrals[9]' , 1.e-4 ) 

print 'Check for integrals[9]                is OK'

for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )

    check_equality ( b ( x1 , y1 , z1  ) , b ( y1 , x1 , z1  ) , "Invalid  value(4)" )
    check_equality ( b ( x1 , y1 , z1  ) , b ( x1 , z1 , y1  ) , "Invalid  value(5)" )
    check_equality ( b ( x1 , y1 , z1  ) , b ( z1 , y1 , x1  ) , "Invalid  value(6)" )

    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f5.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[10]' , 1.e-4 ) 

del f5,_f5
print 'Check for integrals[10]               is OK'



# 2) create & evaluate positive 

BPPM = ROOT.Gaudi.Math.Positive3DMix
b    = BPPM ( 3,  2   , 0 , 5 , 0 , 2 )

sc  = 1.0 
sc *= ( b.xmax() - b.xmin() )
sc *= ( b.ymax() - b.ymin() )
sc *= ( b.zmax() - b.zmin() )
 
for i in  range(50) :
    x = random.uniform ( b.xmin() , b.xmax() )
    y = random.uniform ( b.ymin() , b.ymax() )
    z = random.uniform ( b.zmin() , b.zmax() )
    t = b(x,y,z)
    check_equality ( t , 1/sc , 'Invalid polynom value:' )

print 'Check for positive mixed              is OK'
    

for i in  range ( b.npars() ) : b.setPar(i,random.uniform(-2,2) )


def _f6( x , pars = [] ) :
    return b( x[0] , x[1] ,  x[2] )
f6 = ROOT.TF3('f6',_f6, b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )              
fi = f6.Integral( b.xmin() ,b.xmax() ,  b.ymin(),  b.ymax() ,b.zmin(),  b.zmax() )
i  = b.integral() 
check_equality ( i , fi , 'Invalid integrals[11]' , 1.e-4 ) 

print 'Check for integrals[11]               is OK'

for i in range(50) :
    x1 = random.uniform ( b.xmin() , b.xmax() )
    x2 = random.uniform ( x1       , b.xmax() )
    y1 = random.uniform ( b.ymin() , b.ymax() )
    y2 = random.uniform ( y1       , b.ymax() )
    z1 = random.uniform ( b.zmin() , b.zmax() )
    z2 = random.uniform ( z1       , b.zmax() )

    check_equality ( b ( x1 , y1 , z1  ) , b ( y1 , x1 , z1  ) , "Invalid  value(7)" )

    i1 = b .integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    fi = f6.Integral ( x1 , x2 , y1 , y2 , z1 , z2 )
    check_equality ( i1 , fi , 'Invalid integrals[12]' , 1.e-4 ) 

del f6,_f6
print 'Check for integrals[12]               is OK'



# =============================================================================
# The END 
# =============================================================================
