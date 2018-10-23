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
# $Id$
# =============================================================================
## @file
#
#  Simple file to provide "easy" access in python for
#  the basic ROOT::Math classes
#  @see GaudiKernel/Point3DTypes.h
#  @see GaudiKernel/Vector3DTypes.h
#  @see GaudiKernel/Vector4DTypes.h
#  @see GaudiKernel/GenericVectorTypes.h
#
#  The usage is fairly trivial:
#
#  @code
#
#  import LHCbMath.Types
#
#  @endcode
#
#  Important: All types are defined in corresponding C++ namespaces
#
#  @code
#
#  import LHCbMath.Types
#
#  from GaudiPython.Bindings import gbl as cpp ## get global C++ namespace
#  Gaudi = cpp.Gaudi                           ## get C++ namespace Gaudi
#
#  p3 = Gaudi.XYZPoint(0,1,2)               ## use C++ type Gaudi::XYZPoint
#
#  @endcode
#
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
#
#  Last modification $Date$
#                 by $Author$
#
#
# =============================================================================
"""Simple file to provide 'easy' access in python for the basic ROOT::Math classes

  see $GAUDIKERNELROOT/GaudiKernel/Point3DTypes.h
  see $GAUDIKERNELROOT/GaudiKernel/Vector3DTypes.h
  see $GAUDIKERNELROOT/GaudiKernel/Vector4DTypes.h
  see $GAUDIKERNELROOT/GaudiKernel/GenericVectorTypes.h

  see $LHCBMATHROOT/LHCbMath/Line.h

  The lines and planes are decorated:
     see $LHCBMATHROOT/LHCbMath/GeomFun.h


  The usage is fairly trivial:

  >>> import LHCbMath.Types

  Important: All types are defined in corresponding
               C++ namespaces: Gaudi & Gaudi::Math

  >>> import LHCbMath.Types
  >>> from GaudiPython.Bindings import gbl as cpp ## get global C++ namespace
  >>> Gaudi = cpp.Gaudi                           ## get C++ namespace Gaudi
  >>> p3 = Gaudi.XYZPoint(0,1,2)                  ## use C++ type Gaudi::XYZPoint

  >>> dir( Gaudi.Math )
  >>> dir( Gaudi      )

  Last modification $Date$
                 by $Author$

"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__date__    = "2009-09-12"
__version__ = "Version$Revision$"
# =============================================================================
__all__     = () ## nothing to be imported !
# =============================================================================
import ROOT

try:
    import cppyy
except ImportError:
    # FIXME: backward compatibility
    print "# WARNING: using PyCintex as cppyy implementation"
    import PyCintex as cppyy
    import sys
    sys.modules['cppyy'] = cppyy

## get global C++ namespace
cpp = cppyy.makeNamespace('')

## C++ namespace Gaudi
std   = cpp.std

Gaudi = cpp.Gaudi 
## ROOT::Math namespace
_RM = ROOT.ROOT.Math

## Geometry vectors 
cpp.Gaudi.XYZPoint            = _RM.PositionVector3D     ('ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag')
cpp.Gaudi.XYZVector           = _RM.DisplacementVector3D ('ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag')
cpp.Gaudi.LorentzVector       = _RM.LorentzVector        ('ROOT::Math::PxPyPzE4D<double>')
cpp.Gaudi.Plane3D             = _RM.Plane3D

cpp.Gaudi.Math.XYZPoint       = cpp.Gaudi.XYZPoint
cpp.Gaudi.Math.XYZVector      = cpp.Gaudi.XYZVector
cpp.Gaudi.Math.LorentzVector  = cpp.Gaudi.LorentzVector
cpp.Gaudi.Math.Plane3D        = cpp.Gaudi.Plane3D

cpp.Gaudi.Point3D             = cpp.Gaudi.XYZPoint
cpp.Gaudi.Math.Point3D        = cpp.Gaudi.XYZPoint

cpp.Gaudi.Vector3D            = cpp.Gaudi.XYZVector
cpp.Gaudi.Math.Vector3D       = cpp.Gaudi.XYZVector


# =============================================================================
## try to pickup the vector
@staticmethod
def _vector_ ( i , typ = 'double' ) :
    """Pick up the vector of corresponding size
    >>> V3   = Gaudi.Math.Vector(3)
    >>> vct  = V3 ()
    """
    v        = _RM.SVector ( typ , i )
    return deco_vector ( v ) 

# =============================================================================
## try to pickup the matrix
@staticmethod
def _matrix_ ( i , j , typ = 'double' ) :
    """Pick up the matrix of corresponding size
    >>> M3x4   = Gaudi.Math.Matrix(3,4)
    >>> matrix = M3x4 ()    
    """
    m        = _RM.SMatrix ( "%s,%d,%d" % ( typ , i , j ) )
    ## v.SCALAR = typ 
    return deco_matrix( m )  

# =============================================================================
## try to pickup the symmeric matrix
@staticmethod
def _sym_matrix_ ( i , typ = 'double' ) :
    """Pick up the symmetric matrix of corresponding size
    >>> SymM3  = Gaudi.Math.SymMatrix(3)
    >>> matrix = SymM3 ()
    """
    m = _RM.SMatrix('%s,%d,%d,ROOT::Math::MatRepSym<%s,%d>' %  ( typ , i , i , typ , i ) )
    return deco_symmatrix  ( m ) 

cpp.Gaudi.Vector         =     _vector_
cpp.Gaudi.Math.Vector    =     _vector_
cpp.Gaudi.Matrix         =     _matrix_
cpp.Gaudi.Math.Matrix    =     _matrix_
cpp.Gaudi.SymMatrix      = _sym_matrix_
cpp.Gaudi.Math.SymMatrix = _sym_matrix_

## Gaudi::Math
cpp.Gaudi.Math.XYZLine           = cpp.Gaudi.Math.Line( cpp.Gaudi.XYZPoint, cpp.Gaudi.XYZVector)
cpp.Gaudi.Math.SVector2WithError = cpp.Gaudi.Math.SVectorWithError(2,'double')
cpp.Gaudi.Math.SVector3WithError = cpp.Gaudi.Math.SVectorWithError(3,'double')
cpp.Gaudi.Math.SVector4WithError = cpp.Gaudi.Math.SVectorWithError(4,'double')
cpp.Gaudi.Math.SVector5WithError = cpp.Gaudi.Math.SVectorWithError(5,'double')
cpp.Gaudi.Math.SVector6WithError = cpp.Gaudi.Math.SVectorWithError(6,'double')
cpp.Gaudi.Math.SVector8WithError = cpp.Gaudi.Math.SVectorWithError(8,'double')

cpp.Gaudi.XYZLine                = cpp.Gaudi.Math.XYZLine
cpp.Gaudi.Line3D                 = cpp.Gaudi.Math.XYZLine
cpp.Gaudi.Math.Line3D            = cpp.Gaudi.Math.XYZLine


cpp.Gaudi.Math.frac              = cpp.Gaudi.Math.Functions.frac
cpp.Gaudi.Math.asym              = cpp.Gaudi.Math.Functions.asym
cpp.Gaudi.Math.binomEff          = cpp.Gaudi.Math.Functions.binomEff

## vectors of vectors
cpp.Gaudi.Vectors2       = std.vector ( cpp.Gaudi.Vector2 )
cpp.Gaudi.Vectors3       = std.vector ( cpp.Gaudi.Vector3 )
cpp.Gaudi.Vectors4       = std.vector ( cpp.Gaudi.Vector4 )
cpp.Gaudi.Math.Vectors2  = cpp.Gaudi.Vectors2
cpp.Gaudi.Math.Vectors3  = cpp.Gaudi.Vectors3
cpp.Gaudi.Math.Vectors4  = cpp.Gaudi.Vectors4

cpp.Gaudi.Math.ValueWithError.Vector = cpp.std.vector( cpp.Gaudi.Math.ValueWithError)
cpp.Gaudi.Math.ValueWithError.Vector .__str__   = lambda s : str( [ i for i in s ])
cpp.Gaudi.Math.ValueWithError.Vector .__repr__  = lambda s : str( [ i for i in s ])


## Sum the contents of the vector
def _ve_sum_ ( s ) :
    """Sum the contents of the vector
    >>> v = ...
    >>> s = v.sum()
    """
    return Gaudi.Math.sum ( s )


## Sum the contents of the vector
def _ve_asum_ ( s ) :
    """Sum the contents of the vector
    >>> v = ...
    >>> s = v.abssum()
    """
    return Gaudi.Math.abssum ( s )

_ve_sum_  . __doc__ += '\n' + cpp.Gaudi.Math.sum    .__doc__
_ve_asum_ . __doc__ += '\n' + cpp.Gaudi.Math.abssum .__doc__

_VVE = std.vector( cpp.Gaudi.Math.ValueWithError )

_VVE  . sum      = _ve_sum_
_VVE  . abssum   = _ve_asum_
_VVE  . sumabs   = _ve_asum_
_VVE  . __str__  = lambda s : str( [ i for i in s ] )
_VVE  . __repr__ = lambda s : str( [ i for i in s ] )
_VVE  . __len__  = lambda s : s.size ()

_VVVE = std.vector( _VVE )
_VVVE . __str__  = lambda s : str( [ i for i in s ] )
_VVVE . __repr__ = lambda s : str( [ i for i in s ] )
_VVVE . __len__  = lambda s : s.size ()



_VVE.Vector = _VVVE
cpp.Gaudi.Math.ValueWithError.Vector = _VVE

_C2F            = cpp.Gaudi.Math.Chi2Fit
_C2F . __str__  = lambda s : s.toString ()
_C2F . __repr__ = lambda s : s.toString ()
## chi2-probabilty
def _c2_prob_  ( s ) :
    """Chi2 probabiilty
    >>> r = h.hfit ( ... )
    >>> r.Prob()
    """
    dofs = s.points() - s.size()
    return ROOT.TMath.Prob ( s.chi2() , dofs )

_C2F . Prob        = _c2_prob_
_C2F . Probability = _c2_prob_
_C2F . prob        = _c2_prob_
_C2F . probability = _c2_prob_
_C2F . __len__     = lambda s     : s.size  (   )
_C2F . __getitem__ = lambda s , i : s.param ( i )

# =============================================================================
## self-printout of S-vectors
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _v_str_ ( self , fmt = ' %g' ) :
    """Self-printout of SVectors: (...)
    """
    index  = 0
    result = ''
    while index < self.kSize :
        if 0 != index : result += ', '
        result += fmt % self.At( index )
        index  += 1
    return "( " + result + ' )'

# =============================================================================
## iterator for SVector
#  @code
#  vct = ...
#  for i in vct : print i 
#  @endcode
def _v_iter_ ( self ) :
    """Iterator for SVector
    >>> vct = ...
    >>> for i in vct : print i 
    """
    for i in range(self.kSize) :
        yield self(i)
        
# =============================================================================
## iterator for SVector
#  @code
#  vct = ...
#  for i,v in vct.iteritems() : print i,v 
#  @endcode
def _v_iteritems_ ( self ) :
    """Iterator for SVector
    >>> vct = ...
    >>> for i,v in vct.iteritems() : print i,v 
    """
    for i in range(self.kSize) :
        yield i,self(i)



# =============================================================================
## the multiplication operators 
_mult_ops_ = {}
## get the proper multiplication operator 
def _get_mult_op_ ( klass1 , klass2 ) :
    """Get the proper multiplication operator
    """
    t   = klass1 , klass2
    ops = _mult_ops_.get( t , None )
    if ops : return ops                   ## RETURN  

    ## try to load the operators 
    try :
        ops = cpp.Gaudi.Math.MultiplyOp ( klass1 , klass2 )
        _mult_ops_ [ t ] = ops
        return ops                       ## RETURN 
    except TypeError:
        return None                      ## RETURN
    
    return None                          ## RETURN


# =============================================================================
## equailty operators 
_eq_ops_ = {}
## get the proper multiplication operator 
def _get_eq_op_ ( klass1 , klass2 ) :
    """Get the proper equality operator
    """
    t   = klass1 , klass2
    ops = _eq_ops_.get( t , None )
    if ops : return ops                   ## RETURN  

    ## try to load the operators 
    try :
        ops = cpp.Gaudi.Math.EqualityOp ( klass1 , klass2 )
        _eq_ops_ [ t ] = ops
        return ops                       ## RETURN 
    except TypeError:
        return None                      ## RETURN
    
    return None                          ## RETURN


# =============================================================================
## helper function for Linear Algebra: multiplications
#  multiplication of vectors, matrices, constants 
#  @code
#  vector1 = ...
#  vector2 = ...
#  matrix1 = ...
#  matrix2 = ...
#  print vector1 * vector2 
#  print vector1 * matrix1
#  print matrix1 * vector2
#  print matrix1 * matrix2
#  print vector1 * 2
#  print matrix1 * 2
#  @endcode
def _linalg_mul_ ( a  , b ) :
    """Multiplication of vectors, matrices, etc
    >>> vector1 = ...
    >>> vector2 = ...
    >>> matrix1 = ...
    >>> matrix2 = ...
    >>> print vector1 * vector2 
    >>> print vector1 * matrix1
    >>> print matrix1 * vector2
    >>> print matrix1 * matrix2
    >>> print vector1 * 2
    >>> print matrix1 * 2
    """
    ## simple cases: multiply by a constant 
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v *= b
        return v
    
    ## get the proper operator 
    ops   = _get_mult_op_ ( a.__class__ , b.__class__ )
    if not ops : return NotImplemented 
    return ops.multiply ( a , b )

# =============================================================================
## helper function for "right" multiplication (Linear Algebra)
#  "right multiplication" for a constant
#  @code
#  vector = ...
#  matrix = ...
#  print 2 * vector
#  print 2 * matrix 
#  @endcode 
def _linalg_rmul_ ( a , b ) :
    """``right multiplication'' for a constant
    >>> vector = ...
    >>> matrix = ...
    >>> print 2 * vector
    >>> print 2 * matrix
    """
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v *= b
        return v
    return NotImplemented 

# =============================================================================
## helper function for Linear Algebra divisions 
#  Division by a constant
#  @code
#  vector = ...
#  matrix = ...
#  print vector / 2 
#  print matrix / 2 
#  @endcode 
def _linalg_div_ ( a  , b ) :
    """Division by a constant
    >>> vector = ...
    >>> matrix = ...
    >>> print vector / 2 
    >>> print matrix / 2 
    """
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v /= b
        return v
    return NotImplemented

# =============================================================================
## "cross-product" of two vectors to get a matrix
#  @code 
#  vector1 = ...
#  vector2 = ...
#  matrix =  vector1.cross ( vector2 )
#  @endcode 
def _vector_cross_ ( a, b ) :
    """Cross-product of two vectors to get a matrix
    >>> vector1 = ...
    >>> vector2 = ...
    >>> matrix =  vector1.cross ( vector2 ) 
    """
    ## get the proper operator 
    ops   = _get_mult_op_ ( a.__class__ , b.__class__ )
    if not ops : return NotImplemented 
    return ops.cross ( a , b )


# =============================================================================
## equality of vectors
#  @code
#  vector1 = ...
#  vector2 = ...
#  print vector1 == vector2
#  print vector1 == ( 0, 2, 3 )
#  print vector1 == [ 0, 2, 3 ]
#  @endcode 
def _vector_eq_ ( a , b ) :
    """Equality for vectors
    >>> vector1 = ...
    >>> vector2 = ...
    >>> print vector1 == vector2
    >>> print vector1 == ( 0, 2, 3 )
    >>> print vector1 == [ 0, 2, 3 ]
    """
    if         a    is      b          : return True
    elif not hasattr ( b , '__len__' ) : return False 
    elif  len ( a ) != len ( b )       : return False        
    #
    ops = _get_eq_op_ ( a.__class__ , b.__class__ )
    if ops : return ops.equal ( a , b )  ## RETURN
    ## compare elements  
    for i in range ( len( a ) ) :
        if not _is_equal_ ( a[i] , b[i] ) : return False
        
    return True 
    
# =============================================================================
## equality of matrices
#  @code
#  matrix1 = ...
#  matrix2 = ...
#  print matrix1 == matrix2 
#  @endcode 
def _matrix_eq_ ( a , b ) :
    """Equality for matrices
    >>> matrix1 = ...
    >>> matrix2 = ...
    >>> print matrix1 == matrix2 
    """
    if  a is b : return True
    
    try :
        if   a.kRows != b.kRows : return False
        elif a.kCols != b.kCols : return False
    except :
        pass
        
    ops = _get_eq_op_ ( a.__class__ , b.__class__ )
    if not ops : return NotImplemented
    return ops.equal ( a , b )


# =============================================================================
## decorate vector 
def deco_vector ( t ) :

    if not hasattr ( t , '_decorated' ) :

        t ._old_str_    = t.__str__
        t ._old_repr_   = t.__repr__
        
        t ._old_add_    = t.__add__
        t ._old_radd_   = t.__radd__
        t ._old_mul_    = t.__mul__
        t ._old_rmul_   = t.__rmul__
        t ._old_sub_    = t.__sub__
        t ._old_rsub_   = t.__rsub__
        t ._old_div_    = t.__div__

        _operations   = cpp.Gaudi.Math.VctrOps( t )
        
        t.__add__       = lambda a,b : _operations.add  ( a , b )
        t.__sub__       = lambda a,b : _operations.sub  ( a , b )
        t.__radd__      = lambda a,b : _operations.add  ( a , b )
        t.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        t.__mul__       = _linalg_mul_    
        t.__rmul__      = _linalg_rmul_    
        t.__div__       = _linalg_div_    
        
        t.__eq__        = _vector_eq_     
        t.__neq__       = lambda a,b : not ( a == b ) 
        t.__neg__       = lambda s   : s*(-1) 

        t.cross         = _vector_cross_
        
        t.__rdiv__      = lambda s,*a :  NotImplemented 

        t. _new_str_    = _v_str_
        t. __str__      = _v_str_
        t. __repr__     = _v_str_

        t. __len__      = lambda s : s.kSize 
        t. __contains__ = lambda s, i : 0<=i<s.kSize

        t. __iter__     = _v_iter_        
        t. iteritems    = _v_iteritems_
        
        t. _decorated = True
        
    return t

cpp.Gaudi.Vector2             = cpp.Gaudi.Vector(2)
cpp.Gaudi.Vector3             = cpp.Gaudi.Vector(3)
cpp.Gaudi.Vector4             = cpp.Gaudi.Vector(4)
cpp.Gaudi.Vector5             = cpp.Gaudi.Vector(5)
cpp.Gaudi.Vector6             = cpp.Gaudi.Vector(6)
cpp.Gaudi.Vector8             = cpp.Gaudi.Vector(8)

cpp.Gaudi.Math.Vector2        = cpp.Gaudi.Vector2
cpp.Gaudi.Math.Vector3        = cpp.Gaudi.Vector3
cpp.Gaudi.Math.Vector4        = cpp.Gaudi.Vector4
cpp.Gaudi.Math.Vector5        = cpp.Gaudi.Vector5
cpp.Gaudi.Math.Vector8        = cpp.Gaudi.Vector8


## ============================================================================
## some useful decoration:
## ============================================================================

_V4D = cpp.Gaudi.LorentzVector

## 4-vectors 
def _v4_iadd_ ( s , other ) :
    s.SetE    ( s.E  () + other.E  () )
    s.SetPx   ( s.Px () + other.Px () )
    s.SetPy   ( s.Py () + other.Py () )
    s.SetPz   ( s.Pz () + other.Pz () )
    return s

def _v4_isub_ ( s , other ) :
    s.SetE    ( s.E  () - other.E  () )
    s.SetPx   ( s.Px () - other.Px () )
    s.SetPy   ( s.Py () - other.Py () )
    s.SetPz   ( s.Pz () - other.Pz () )
    return s

def _v4_dot_   ( s , other ) :
    res  = s.e  () * other.e  () 
    res -= s.px () * other.px ()
    res -= s.py () * other.py ()
    res -= s.pz () * other.pz ()
    return res 

if not hasattr ( _V4D , '__iadd__' ) : _V4D. __iadd__ = _v4_iadd_ 
if not hasattr ( _V4D , '__isub__' ) : _V4D. __isub__ = _v4_isub_ 
if not hasattr ( _V4D , 'Dot'      ) : _V4D.Dot       = _v4_dot_

def _v4_mul_ ( self , other ) :
    """Multiplication/scaling of Lorentz Vectors 
    >>> vct = ...
    >>> a   = vct * 2
    
    >>> vct2 = ...
    >>> prod = vct * vct2 ## NB! 
    """
    if isinstance ( other , _V4D ) : return self.Dot ( other )
    #
    tmp   = _V4D ( self )
    tmp  *= other
    return tmp

def _v4_add_ ( self , other ) :
    """ Addition of Lorentz Vectors 
    >>> vct1 = ...
    >>> vct2 = ...
    >>> a    = vct1 + vct2
    """
    tmp   = _V4D ( self )
    tmp  += other
    return tmp

def _v4_sub_ ( self , other ) :
    """Subtraction of Lorentz Vectors 
    >>> vct1 = ...
    >>> vct2 = ...
    >>> a    = vct1 - vct2
    """
    tmp   = _V4D ( self )
    tmp  -= other
    return tmp

def _v4_div_ ( self , other ) :
    """Division/scaling of Lorentz Vectors     
    >>> vct = ...
    >>> a   = vct / 2 
    """
    tmp   = _V4D ( self )
    tmp  /= other
    return tmp

_V4D . __mul__  = _v4_mul_
_V4D . __add__  = _v4_add_
_V4D . __sub__  = _v4_sub_
_V4D . __div__  = _v4_div_


_V4D . __radd__ = lambda s,o : s+o 
_V4D . __rmul__ = lambda s,o : s*o 

_V4 = cpp.Gaudi.Math.Vector4
# =============================================================================
## convert LorentzVector into SVector
#  @code
#  lv = ...
#  v4 = lv.asSVector()
#  @endcode 
def _v4_as_v4_ ( self ) :
    """Convert LorentzVector into SVector
    >>> lv = ...
    >>> v4 = lv.asSVector()
    """
    v4 = _V4()
    v4[0] = self.X()
    v4[1] = self.Y()
    v4[2] = self.Z()
    v4[3] = self.T()
    return _v4 

_V4D.asSVector = _v4_as_v4_ 


## 3-vectors 
_P3D = cpp.Gaudi.XYZPoint
_V3D = cpp.Gaudi.XYZVector

## 3-vectors & points

def _v3_iadd_  ( s , other ) :
    s.SetX     ( s.X () + other.X () )
    s.SetY     ( s.Y () + other.Y () )
    s.SetZ     ( s.Z () + other.Z () )
    return s

def _v3_isub_  ( s , other ) :
    s.SetX     ( s.X () - other.X () )
    s.SetY     ( s.Y () - other.Y () )
    s.SetZ     ( s.Z () - other.Z () )
    return s

def _v3_dot_   ( s , other ) :
    res  = s.X ( ) * other.X ( )
    res -= s.Y ( ) * other.Y ( )
    res -= s.Z ( ) * other.Z ( )
    return res 

if not hasattr ( _V3D , '__iadd__' ) : _V3D. __iadd__ = _v3_iadd_
if not hasattr ( _V3D , '__isub__' ) : _V3D. __isub__ = _v3_isub_
if not hasattr ( _V3D , 'Dot'      ) : _V3D.Dot       = _v3_dot_
## _V3D. __iadd__ = _v3_iadd_
## _V3D. __isub__ = _v3_isub_
## _V3D.Dot       = _v3_dot_

if not hasattr ( _P3D , '__iadd__' ) : _P3D. __iadd__ = _v3_iadd_ 
if not hasattr ( _P3D , '__isub__' ) : _P3D. __isub__ = _v3_isub_ 
## _P3D. __iadd__ = _v3_iadd_ 
## _P3D. __isub__ = _v3_isub_ 


def _p3_as_v3_ ( self ) :
    """ Conversion to 3D-vector"""
    return _V3D( self.x() , self.y() , self.z() )
def _v3_as_p3_ ( self ) :
    """ Conversion to 3D-point """    
    return _P3D( self.x() , self.y() , self.z() )

_P3D. asV3 = _p3_as_v3_
_V3D. asP3 = _v3_as_p3_

_V3 = cpp.Gaudi.Math.Vector3
# =============================================================================
## convert 3D-Vector/3D-point into SVector
#  @code
#  lv = ...
#  v4 = lv.asSVector()
#  @endcode 
def _v3_as_v3_ ( self ) :
    """Convert 3D-Vector/3D into SVector
    >>> lv = ...
    >>> v3 = lv.asSVector()
    """
    _v3 = _V3()
    _v3[0] = self.X()
    _v3[1] = self.Y()
    _v3[2] = self.Z()
    return _v3

_V3D.asSVector = _v3_as_v3_ 
_P3D.asSVector = _v3_as_v3_ 

# =============================================================================
def _p3_add_ ( self , other ) :
    """Addition of 3D-point and 3D-vector    
    >>> point  = ...
    >>> vector = ...
    >>> result = point + vector 
    """
    # POINT + VECTOR = POINT 
    if isinstance ( other , _V3D ) :
        tmp   = _P3D ( self )
        tmp  += other
        return tmp
    #
    return NotImplemented


# =============================================================================
def _p3_sub_ ( self , other ) :
    """Substraction of 3D-points 
    
    >>> point1 = ...
    >>> point2 = ...
    >>> vector = ...
    >>> result_point  = point1 - vector
    >>> result_vector = point1 - point2  
    """
    # POINT - VECTOR = POINT
    if   isinstance ( other , _V3D ) :
        tmp   = _P3D ( self )
        tmp  -= other
        return tmp
    # POINT - POINT = VECTOR 
    elif isinstance ( other , _P3D ) :
        tmp   = _V3D (  self.x() ,  self.y() ,  self.z() )
        ## tmp  -= other
        tmp  -= _V3D ( other.x() , other.y() , other.z() ) 
        return tmp
    #
    return NotImplemented 

# =============================================================================
def _p3_mul_ ( self , other ) :
    """Scaling of 3D-points 
    
    >>> point  = ...
    >>> result = point1 * 2 
    """
    tmp  = _P3D ( self )
    tmp *= other
    return tmp

# =============================================================================
def _p3_div_ ( self , other ) :
    """Scaling of 3D-points 
    
    >>> point  = ...
    >>> result = point1 / 2 
    """
    tmp  = _P3D ( self )
    tmp /= other
    return tmp
    
# =============================================================================
def _v3_add_ ( self , other ) :
    """Addition  of 3D-vectors
    
    >>> vector1 = ...
    >>> vector2 = ...
    >>> result_vector = vector1 + vector2
    >>> point   =
    >>> result_point  = vector1 + point 
    """
    # VECTOR + VECTOR = VECTOR 
    if   isinstance ( other , _V3D ) :
        tmp   = _V3D ( self )
        tmp  += other
        return tmp
    # VECTOR + POINT  = POINT 
    elif isinstance ( other , _P3D ) : return other + self
    #
    return NotImplemented 

# =============================================================================
def _v3_sub_ ( self , other ) :
    """Subtraction  of 3D-vectors
    
    >>> vector1 = ...
    >>> vector2 = ...
    >>> result_vector = vector1 - vector2
    """
    # VECTOR - VECTOR = VECTOR 
    if   isinstance ( other , _V3D ) :
        tmp   = _V3D ( self )
        tmp  -= other
        return tmp
    #
    return NotImplemented 

# =============================================================================
def _v3_mul_ ( self , other ) :
    """Multiplication  of 3D-vectors
    
    >>> vector1 = ...
    >>> result  = vector1 * 2 
    >>> vector2 = ...
    >>> product = vector1 * vector2
    """
    # VECTOR * VECTOR = NUMBER
    if   isinstance ( other , _V3D ) : return self.Dot ( other )
    # VECTOR * NUMBER = NUMBER 
    elif isinstance ( other , ( float , int , long ) ) :  
        tmp  = _V3D ( self )
        tmp *= other
        return tmp
    #
    return NotImplemented

# =============================================================================
def _v3_div_ ( self , other ) :
    """Scaling of 3D-vectors
    
    >>> vector = ...
    >>> result = vector1 / 2 
    """
    tmp  = _V3D ( self )
    tmp /= other
    return tmp

_P3D . __add__  = _p3_add_
_P3D . __sub__  = _p3_sub_
_P3D . __div__  = _p3_div_
_P3D . __mul__  = _p3_mul_

_V3D . __add__  = _v3_add_
_V3D . __sub__  = _v3_sub_
_V3D . __div__  = _v3_div_
_V3D . __mul__  = _v3_mul_

_P3D . __radd__ = lambda s,o : s+o 
_P3D . __rmul__ = lambda s,o : s*o 
_V3D . __radd__ = lambda s,o : s+o 
_V3D . __rmul__ = lambda s,o : s*o 



# =============================================================================
def _v4_pow_ ( self , e ) :
    """Squared length of the 3D-vector 
    """
    if 2 != e : return NotImplemented
    return self.M2   ()


# =============================================================================
def _v3_pow_ ( self , e ) :
    """Squared length of the 3D-vector 
    """
    if 2 != e : return NotImplemented
    return self.Mag2 ()

_V4D.__pow__ = _v4_pow_
_V3D.__pow__ = _v3_pow_
_P3D.__pow__ = _v3_pow_


# =============================================================================
## Self-printout of 3D-points and 3D-vectors
def _v3_str_ ( self , fmt = "(%g,%g,%g) ") :
    """Self-printout of 3D-points and 3D-vectors
    """
    return fmt % ( self.X() , self.Y( ), self.Z() )

# =============================================================================
## Self-printout of 4D-vectors
def _v4_str_ ( self , fmt = "[(%g,%g,%g),%g]" ) :
    """Self-printout of 4D-vectors
    """
    return fmt % ( self.X() , self.Y( ), self.Z() , self.E() )


# =============================================================================
if not hasattr ( _P3D , '_new_str_' ) :
    _P3D . _new_str_ = _v3_str_
    _P3D . __str__   = _v3_str_
    _P3D . __repr__  = _v3_str_

# =============================================================================
if not hasattr ( _V3D , '_new_str_' ) :
    _V3D . _new_str_ = _v3_str_
    _V3D . __str__   = _v3_str_
    _V3D . __repr__  = _v3_str_

# =============================================================================
if not hasattr ( _V4D , '_new_str_' ) :
    _V4D . _new_str_ = _v4_str_
    _V4D . __str__   = _v4_str_
    _V4D . __repr__  = _v4_str_

# =============================================================================
## Self-printout of line
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _l_str_ ( self ) :
    """Self-printout of line: (point, direction)
    >>> line = ... 
    >>> print line 
    """
    return "(%s,%s)" % ( self.beginPoint() , self.direction() )

if not hasattr ( cpp.Gaudi.Math.XYZLine , '_new_str_' ) :
    cpp.Gaudi.Math.XYZLine._new_str_ = _l_str_
    cpp.Gaudi.Math.XYZLine.__str__   = _l_str_
    cpp.Gaudi.Math.XYZLine.__repr__  = _l_str_

# ============================================================================
## self-printout of matrices
def _mg_str_ ( self , fmt = ' %+11.4g') :
    """Self-printout of matrices
    >>> matrix = ...
    >>> print matrix 
    """
    _rows = self.kRows
    _cols = self.kCols
    _line = ''
    for _irow in range ( 0 , _rows ) :
        _line += ' |'
        for _icol in range ( 0 , _cols ) :
            _line += fmt % self( _irow , _icol )
        _line += ' |'
        if ( _rows - 1 )  != _irow : _line += '\n'
    return _line
#
## self-printout of symmetrical matrices
def _ms_str_ ( self , fmt = ' %+11.4g' , width = 12 ) :
    """Self-printout of symmetrical matrices
    >>> matrix = ...
    >>> print matrix 
    """
    _rows = self.kRows
    _cols = self.kCols
    _line = ''
    for _irow in range ( 0 , _rows ) :
        _line += ' |'
        for _icol in range ( 0 , _cols  ) :
            if _icol < _irow : _line += width*' '
            else             : _line += fmt % self( _irow , _icol )
        _line += ' |'
        if ( _rows - 1 )  != _irow : _line += '\n'
    return _line

# =============================================================================
## get the correlation matrix
def _m_corr_ ( self ) :
    """Get the correlation matrix
    >>> mtrx = ...
    >>> corr = mtrx.correlations()
    """
    from math import sqrt

    _t = type ( self )
    _c = _t   ()
    _rows = self.kRows
    for i in range ( 0 , _rows ) :
        _dI = sqrt ( self ( i , i ) )
        for j in range ( i + 1 , _rows ) :
            _dJ = sqrt ( self ( j , j ) )
            _c [ i , j ] = self ( i , j ) /  (  _dI * _dJ )
        _c[ i , i ] = 1.0

    return _c

# =============================================================================
## "getter"
def _m_get_ ( o , i , j ) :

    try :
        return o ( i , j )
    except :
        pass
    
    try :
        return o [ i , j ]
    except :
        pass

    return o [ i ][ j ]
        
# =============================================================================
## add some matrix-like object to the matrix
#  @code
#  m = ...
#  o = ...
#  m.add_to  ( o ) 
#  @endcode
def _mg_increment_ ( m , o ) : 
    """ Add some ``matrix-like'' object to the matrix
    >>> m = ...
    >>> o = ...
    >>> m.increment  ( o ) 
    """
    for i in range ( m.kRows ) :
        for j in range ( m.kCols ) :
            m[i,j] = m(i,j) + _m_get_ ( o , i , j )
                    
    return m

# =============================================================================
## add some matrix-like object to the matrix
#  @code
#  m = ...
#  o = ...
#  m.add_to  ( o ) 
#  @endcode
def _ms_increment_ ( m , o ) : 
    """ Add some ``matrix-like'' object to the matrix
    >>> m = ...
    >>> o = ...
    >>> m.increment  ( o ) 
    """
    for i in range ( m.kRows ) :
        for j in range ( i , m.kCols ) :
            m[i,j] = m(i,j) + 0.5 * ( _m_get_ ( o , i , j ) + _m_get_ ( o , j , i ) )  
                    
    return m

# =============================================================================
## iterator for SMatrix
#  @code
#  matrix = ...
#  for i in matrix : print i 
#  @endcode
def _m_iter_ ( self ) :
    """Iterator for SMatrix
    >>> matrix = ...
    >>> for i in matrix : print i 
    """
    for i in range(self.kRows) :
        for j in range(self.kCols) :
            yield self(i,j)

# =============================================================================
## iterator for SMatrix
#  @code
#  matrix = ...
#  for i,j,v in matrix.iteritems() : print i,j,v 
#  @endcode
def _m_iteritems_ ( self ) :
    """Iterator for SMatrix
    >>> matrix = ...
    >>> for i,j,v in matrix.iteritems() : print i,j,v
    """
    for i in range(self.kRows) :
        for j in range(self.kCols) :
            yield i,j,self(i,j)

        
# =============================================================================
## construct ``similarity'' with ``vector-like'' object
#  @code
#  m = ...
#  v = ...
#  m.sim ( v )
def  _ms_sim_ ( m , v ) :
    """ construct ``similarity'' with ``vector-like'' object
    >>> m = ...
    >>> v = ...
    >>> m.sim ( v )
    """
    sim = 0.0
    for i in range(m.kRows) :
        for j in range(m.kCols) :
            sim += v[i]*m(i,j)*v[j] 
    return sim 
            
        
##  decorate the matrix  type 
def deco_matrix ( m  ) :
    
    if not hasattr ( m , '_decorated' ) :

        ##  save 'old method'
        m. _old_str_   = m . __str__
        m. _old_repr_  = m . __repr__

        m. _new_str_   = _mg_str_
        m. __repr__    = _mg_str_
        m. __str__     = _mg_str_
        
        m ._old_add_    = m.__add__
        m ._old_radd_   = m.__radd__
        m ._old_mul_    = m.__mul__
        m ._old_rmul_   = m.__rmul__
        m ._old_sub_    = m.__sub__
        m ._old_rsub_   = m.__rsub__
        m ._old_div_    = m.__div__
        
        _operations   = cpp.Gaudi.Math.MtrxOps( m )
        
        m.__add__       = lambda a,b : _operations.add  ( a , b )
        m.__sub__       = lambda a,b : _operations.sub  ( a , b )
        m.__radd__      = lambda a,b : _operations.add  ( a , b )
        m.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        m.__mul__       = _linalg_mul_    
        m.__rmul__      = _linalg_rmul_    
        m.__div__       = _linalg_div_     

        m.__eq__        = _matrix_eq_     
        m.__neq__       = lambda a,b : not ( a == b ) 
        m.__neg__       = lambda s   : s*(-1)
        
        m.__rdiv__      = lambda s,*a :  NotImplemented 

        
        m._increment_  = _mg_increment_
        m. increment   = _mg_increment_

        m.__iter__     = _m_iter_
        m.iteritems    = _m_iteritems_
        m.__contains__ = lambda s,ij : 0<=ij[0]<s.kRows and 0<=ij[1]<s.kCols
        
        m._decorated   = True
        
    return m

##  decorate the symmetrix matrix  type 
def deco_symmatrix ( m ) :

    if not hasattr ( m , '_decorated' ) :

        ##  save 'old method'
        m. _old_str_   = m . __str__
        m. _old_repr_  = m . __repr__

        m.correlations = _m_corr_
        m. _new_str_   = _ms_str_
        m. __repr__    = _ms_str_
        m. __str__     = _ms_str_

        m ._old_add_    = m.__add__
        m ._old_radd_   = m.__radd__
        m ._old_mul_    = m.__mul__
        m ._old_rmul_   = m.__rmul__
        m ._old_sub_    = m.__sub__
        m ._old_rsub_   = m.__rsub__
        m ._old_div_    = m.__div__

        _operations   = cpp.Gaudi.Math.MtrxOps( m )
        
        m.__add__       = lambda a,b : _operations.add  ( a , b )
        m.__sub__       = lambda a,b : _operations.sub  ( a , b )
        m.__radd__      = lambda a,b : _operations.add  ( a , b )
        m.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        m.__mul__       = _linalg_mul_    
        m.__rmul__      = _linalg_rmul_    
        m.__div__       = _linalg_div_     

        m.__eq__        = _matrix_eq_     
        m.__neq__       = lambda a,b : not ( a == b ) 
        m.__neg__       = lambda s   : s*(-1)

        m.__rdiv__      = lambda s,*a :  NotImplemented 

        m._increment_  = _ms_increment_
        m.increment    = _ms_increment_

        m.__iter__     = _m_iter_
        m.iteritems    = _m_iteritems_
        m.__contains__ = lambda s,ij : 0<=ij[0]<s.kRows and 0<=ij[1]<s.kCols
        
        m._sim_        = _ms_sim_
        m.sim          = _ms_sim_

        m._decorated   = True
        
    return m

cpp.Gaudi.SymMatrix2x2        = cpp.Gaudi.SymMatrix(2)
cpp.Gaudi.SymMatrix3x3        = cpp.Gaudi.SymMatrix(3)
cpp.Gaudi.SymMatrix4x4        = cpp.Gaudi.SymMatrix(4)
cpp.Gaudi.SymMatrix5x5        = cpp.Gaudi.SymMatrix(5)
cpp.Gaudi.SymMatrix6x6        = cpp.Gaudi.SymMatrix(6)
## LHCb::Particle
cpp.Gaudi.SymMatrix7x7        = cpp.Gaudi.SymMatrix(7)
## Gaudi::Math::ParticleParams
cpp.Gaudi.SymMatrix8x8        = cpp.Gaudi.SymMatrix(8)
## LHCb:TwoProngVertex
cpp.Gaudi.SymMatrix9x9        = cpp.Gaudi.SymMatrix(9)


cpp.Gaudi.Math.SymMatrix2x2   = cpp.Gaudi.SymMatrix2x2
cpp.Gaudi.Math.SymMatrix3x3   = cpp.Gaudi.SymMatrix3x3
cpp.Gaudi.Math.SymMatrix4x4   = cpp.Gaudi.SymMatrix4x4
cpp.Gaudi.Math.SymMatrix5x5   = cpp.Gaudi.SymMatrix5x5
cpp.Gaudi.Math.SymMatrix6x6   = cpp.Gaudi.SymMatrix6x6
cpp.Gaudi.Math.SymMatrix7x7   = cpp.Gaudi.SymMatrix7x7
cpp.Gaudi.Math.SymMatrix8x8   = cpp.Gaudi.SymMatrix8x8
cpp.Gaudi.Math.SymMatrix9x9   = cpp.Gaudi.SymMatrix9x9

#
# specific matrices for 'tracks'
#

cpp.Gaudi.Matrix5x5             = cpp.Gaudi.Matrix(5,5)
cpp.Gaudi.TrackMatrix           = cpp.Gaudi.Matrix5x5
cpp.Gaudi.Math.Matrix5x5        = cpp.Gaudi.Matrix5x5
cpp.Gaudi.Math.TrackMatrix      = cpp.Gaudi.TrackMatrix

cpp.Gaudi.TrackSymMatrix        = cpp.Gaudi.SymMatrix5x5
cpp.Gaudi.Math.TrackSymMatrix   = cpp.Gaudi.TrackSymMatrix

cpp.Gaudi.TrackVector           = cpp.Gaudi.Vector5
cpp.Gaudi.Math.TrackVector      = cpp.Gaudi.TrackVector

#
# matrix from LHCb::Particle
#
cpp.Gaudi.Matrix4x3             = cpp.Gaudi.Matrix(4,3)
cpp.Gaudi.Math.Matrix4x3        = cpp.Gaudi.Matrix4x3


for i in range(11) :
    
    t0 = cpp.Gaudi.Vector ( i )
    deco_vector    ( t0 )
    
    t1 = cpp.Gaudi.SymMatrix(i)
    deco_symmatrix ( t1 )
    
    for j in range(11) : 
        t2 = cpp.Gaudi.Matrix(i,j)
        deco_matrix ( t2 ) 

# =============================================================================
## Self-printout of 3D-plane
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _p_str_ ( self ) :
    """Self-printout of 3D-plane: (point, normal)
    >>> plane = ...
    >>> print plance 
    """
    return "(%s,%s)" % ( self.ProjectOntoPlane( cpp.Gaudi.XYZPoint()) , self.Normal() )

if not hasattr ( cpp.Gaudi.Plane3D , '_new_str_' ) :
    cpp.Gaudi.Plane3D._new_str_ = _p_str_
    cpp.Gaudi.Plane3D.__str__   = _p_str_
    cpp.Gaudi.Plane3D.__repr__  = _p_str_

for t in ( cpp.Gaudi.Math.ValueWithError         ,
           cpp.Gaudi.Math.Point3DWithError       ,
           cpp.Gaudi.Math.Vector3DWithError      ,
           cpp.Gaudi.Math.LorentzVectorWithError ,
           cpp.Gaudi.Math.SVector2WithError      ,
           cpp.Gaudi.Math.SVector3WithError      ,
           cpp.Gaudi.Math.SVector4WithError      ,
           cpp.Gaudi.Math.SVector5WithError      ,
           cpp.Gaudi.Math.SVector6WithError      ,
           cpp.Gaudi.Math.SVector8WithError      ) :
    if not hasattr ( t , '_new_str_' ) :
        t._new_str_ = t.toString
        t.__str__   = t.toString
        t.__repr__  = t.toString


cpp.Gaudi.Math.SVector2WithError  . __len__ = lambda s : 2 
cpp.Gaudi.Math.SVector3WithError  . __len__ = lambda s : 3 
cpp.Gaudi.Math.SVector4WithError  . __len__ = lambda s : 4 
cpp.Gaudi.Math.SVector5WithError  . __len__ = lambda s : 5 
cpp.Gaudi.Math.SVector6WithError  . __len__ = lambda s : 6 
cpp.Gaudi.Math.SVector8WithError  . __len__ = lambda s : 8 

# =============================================================================
## get the eigenvalues for symmetric matrices :
def _eigen_1_ ( self , sorted = True ) :
    """Get the eigenvalues for symmetric matrices :
    >>> mtrx = ...
    >>> values = mtrx.eigenValues ( sorted = True )
    """
    return cpp.Gaudi.Math.EigenSystems.eigenValues ( self , sorted )

# =============================================================================
## get the eigevectors for symmetric matrices :
def _eigen_2_ ( self , sorted = True ) :
    """Get the eigevectors for symmetric matrices :
    >>> mtrx = ...
    >>> values, vectors = mtrx.eigenVectors( sorted = True )
    """
    if   2 == self.kCols :
        _values  = cpp.Gaudi.Vector2  ()
        _vectors = cpp.Gaudi.Vectors2 ()
    elif 3 == self.kCols :
        _values  = cpp.Gaudi.Vector3  ()
        _vectors = cpp.Gaudi.Vectors3 ()
    elif 4 == self.kCols :
        _values  = cpp.Gaudi.Vector4  ()
        _vectors = cpp.Gaudi.Vectors4 ()
    else :
        raise AttributeError, "Not implemented for dimention: %s" % self.kCols

    st = cpp.Gaudi.Math.EigenSystems.eigenVectors ( self , _values , _vectors , sorted )
    if st.isFailure () :
        print 'EigenVectors: Failure from EigenSystems' , st

    return ( _values , _vectors )


_eigen_1_ .__doc__ += '\n' +  cpp.Gaudi.Math.EigenSystems.eigenValues  . __doc__
_eigen_2_ .__doc__ += '\n' +  cpp.Gaudi.Math.EigenSystems.eigenVectors . __doc__

for m in ( cpp.Gaudi.SymMatrix2x2   ,
           cpp.Gaudi.SymMatrix3x3   ,
           cpp.Gaudi.SymMatrix4x4   ) :
    if not hasattr ( m , 'eigenValues'  ) : m.eigenValues  = _eigen_1_
    if not hasattr ( m , 'eigenVectors' ) : m.eigenVectors = _eigen_2_


# =============================================================================
## self-printout of Gaudi::Math::ParticleParams
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2010-05-26
def _pp_str_ ( self ) :
    """Self-printout of ParticleParams
    """
    pos = self.position    ()
    mom = self.momentum    ()
    len = self.decayLength ()
    return " ( %s/%s/%s ) " % ( pos, mom , len )

if not hasattr ( cpp.Gaudi.Math.ParticleParams , '_new_str_' ) :
    cpp.Gaudi.Math.ParticleParams._new_str_ = _pp_str_
    cpp.Gaudi.Math.ParticleParams.__str__   = _pp_str_
    cpp.Gaudi.Math.ParticleParams.__repr__  = _pp_str_

# =============================================================================
## various decorators for GeomFun.h
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
if not hasattr ( cpp.Gaudi.Math , 'XYZGeomFun' ) :
    cpp.Gaudi.Math.XYZGeomFun = cpp.Gaudi.Math.GF(
        cpp.Gaudi.XYZPoint  ,
        cpp.Gaudi.XYZLine   ,
        cpp.Gaudi.Plane3D
        )
if not hasattr ( cpp.Gaudi , 'XYZGeomFun' ) :
    cpp.Gaudi.XYZGeomFun = cpp.Gaudi.Math.XYZGeomFun

_GeomFun = cpp.Gaudi.Math.XYZGeomFun

# =============================================================================
## intersection of line and plane
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_line_and_plane_ ( line , plane ) :
    """Find the intersection of line and plane

    >>> line  = ...
    >>> plane = ...

    >>> ok, point, mu = line.intersect ( plane )

    The return value is a tuple:
    - the point
    - the parameter along the line
    - the flag (true if intersection exists)

    """
    _point = cpp.Gaudi.XYZPoint(0,0,-1.e+10)
    _mu    = ROOT.Double(-1.e+10)
    _flag  = _GeomFun.intersection ( line   ,
                                     plane  ,
                                     _point ,
                                     _mu    )
    if _flag : _flag = True
    else     : _flag = False
    return (_point,_mu,_flag)

_intersect_line_and_plane_ . __doc__ += '\n' + _GeomFun.intersection . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'intersect' ) :
    cpp.Gaudi.XYZLine.intersect = _intersect_line_and_plane_

# =============================================================================
## intersect two planes
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_two_planes_ ( plane , plane1 ) :
    """Find the intersection line for two planes:

    >>> plane  = ...
    >>> plane1 = ...
    >>> line, flag = plane.line(plane1)

    Return value is a tuple:

    - the intersection line
    - the flag (true if intersection exists)

    """
    _line = cpp.Gaudi.XYZLine()
    _flag = _GeomFun.intersection ( plane , plane1 , _line )
    if _flag : _flag = True
    else     : _flag = False
    return (_line,_flag)

_intersect_two_planes_ . __doc__ += '\n' + _GeomFun.intersection . __doc__

if not hasattr ( cpp.Gaudi.Plane3D , 'line' ) :
    cpp.Gaudi.Plane3D.line = _intersect_two_planes_


# =============================================================================
## intersect three planes
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_three_planes_ ( plane , plane1 , plane2 ) :
    """
    Find the intersection point for three planes:

    >>> plane  = ...
    >>> plane1 = ...
    >>> plane3 = ...
    >>> point, flag = plane.point(plane1,plane2)

    Return value is a tuple:

    - the intersection point
    - the flag (true if intersection exists)

    """
    _point = cpp.Gaudi.XYZPoint(0,0,-1.e+10)
    _flag = _GeomFun.intersection ( plane , plane1 , plane2 , _point )
    if _flag : _flag = True
    else     : _flag = False
    return (_point,_flag)


_intersect_three_planes_ . __doc__ += '\n' + _GeomFun.intersection . __doc__

if not hasattr ( cpp.Gaudi.Plane3D , 'point' ) :
    cpp.Gaudi.Plane3D.point = _intersect_three_planes_


# =============================================================================
## intersect the planes
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_the_planes_ ( plane , plane1 , plane2 = None ) :
    """Find the intersection line/point for two or three planes:

    >>> plane  = ...
    >>> plane1 = ...
    >>> line, flag = plane.intersect(plane1)

    >>> plane  = ...
    >>> plane1 = ...
    >>> plane2 = ...
    >>> point, flag = plane.intersect(plane1,plane2)

    Return value is a tuple:

    - the intersection line/point
    - the flag (true if intersection exists)

    """
    if not plane2 : return _intersect_two_planes_ ( plane , plane1 )
    return _intersect_three_planes_ ( plane , plane1 , plane2 )

_intersect_the_planes_ . __doc__ += '\n' + _GeomFun.intersection . __doc__

if not hasattr ( cpp.Gaudi.Plane3D , 'intersect' ) :
    cpp.Gaudi.Plane3D.intersect = _intersect_the_planes_


# =============================================================================
## calculate the impact parameter of the line & point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _imp_par_1_ ( line , point ) :
    """Calculate the impact parameter of the line and the point
    >>> line  = ...
    >>> point = ...
    >>> ip = line.impactParameter ( point )
    """
    return _GeomFun.impactParameter ( point , line )

_imp_par_1_ . __doc__ += '\n' + _GeomFun.impactParameter . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'impactParameter' ) :
    cpp.Gaudi.XYZLine.impactParameter = _imp_par_1_
if not hasattr ( cpp.Gaudi.XYZLine , 'ip'              ) :
    cpp.Gaudi.XYZLine.ip              = _imp_par_1_

# =============================================================================
## calculate the impact parameter of the line & point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _imp_par_2_ ( point , line ) :
    """Calculate the impact parameter of the line and the point
    >>> point = ...
    >>> line  = ...
    >>> ip = point.impactParameter ( line )
    """
    return _GeomFun.impactParameter ( point , line )


_imp_par_2_ . __doc__ += '\n' + _GeomFun.impactParameter . __doc__

if not hasattr ( cpp.Gaudi.XYZPoint , 'impactParameter' ) :
    cpp.Gaudi.XYZPoint.impactParameter = _imp_par_2_
if not hasattr ( cpp.Gaudi.XYZPoint , 'ip'              ) :
    cpp.Gaudi.XYZPoint.ip              = _imp_par_2_

# =============================================================================
## distance between two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _distance_between_two_lines_ ( line , line1 ) :
    """Find the distance between two lines :
    >>> line  = ...
    >>> line1 = ...
    >>> dist = line.distance ( line1 )
    """
    return _GeomFun.distance ( line , line1 )

_distance_between_two_lines_ . __doc__ += '\n' + _GeomFun.distance. __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'distance' ) :
    cpp.Gaudi.XYZLine.distance = _distance_between_two_lines_


# =============================================================================
## find the closest points for two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_points_ ( line , line1 ) :
    """Calculate the two closest points between two lines

    >>> line1 = ...
    >>> line2 = ...
    >>> point1 , point2 , flag = line1.closestPoints ( line2 )

    The return values is a tuple:
    - the point onthe fist line
    - the point on the second line
    - the flag (true is everything OK)
    """
    _point1 = cpp.Gaudi.XYZPoint(0,0,-1.e+10)
    _point2 = cpp.Gaudi.XYZPoint(0,0,-1.e+11)
    _flag   = _GeomFun.closestPoints ( line , line1 , _point1 , _point2 )
    if    _flag : _flag = True
    else        : _flag = False
    return (_point1,_point2,_flag)


_closest_points_ . __doc__ += '\n' + _GeomFun.closestPoints . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'closestPoints' ) :
   cpp.Gaudi.XYZLine.closestPoints = _closest_points_


# =============================================================================
## find the closest points for two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_params_ ( line , line1 ) :
    """Calculate the parameters for two closest points between two lines

    >>> line1 = ...
    >>> line2 = ...
    >>> mu1 , mu2 , flag = line1.closestPointParams ( line2 )

    The return values is a tuple:
    - the 'mu-parameter of closest point along the first  line
    - the 'mu-parameter of closest point along the second line
    - the flag (true is everything OK)
    """
    _mu1    = ROOT.Double(-1.e+10)
    _mu2    = ROOT.Double(-1.e+11)
    _flag   = _GeomFun.closestPointParams ( line , line1 , _mu1 , _mu2 )
    if    _flag : _flag = True
    else        : _flag = False
    return (_mu1,_mu2,_flag)


_closest_point_params_ . __doc__ += '\n' + _GeomFun.closestPointParams . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'closestPointParams' ) :
    cpp.Gaudi.XYZLine.closestPointParams = _closest_point_params_

# =============================================================================
## find the point on ilne closest to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_1_ ( line , point ) :
    """Find the point on line closest to the given point
    >>> line  = ...
    >>> point = ...
    >>> ClosestPoint  = line.closestPoint ( point )
    """
    return _GeomFun.closestPoint ( point , line )

_closest_point_1_ . __doc__ += '\n' + _GeomFun.closestPoint . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'closestPoint' ) :
    cpp.Gaudi.XYZLine.closestPoint = _closest_point_1_

# =============================================================================
## find the point on ilne closest to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_2_ ( point , line ) :
    """Find the point on line closest to the given point
    >>> point = ...
    >>> line  = ...
    >>> ClosestPoint  = point.closestPoint ( line )
    """
    return _GeomFun.closestPoint ( point , line )

_closest_point_2_ . __doc__ += '\n' + _GeomFun.closestPoint . __doc__

if not hasattr ( cpp.Gaudi.XYZPoint , 'closestPoint' ) :
    cpp.Gaudi.XYZPoint.closestPoint = _closest_point_2_


# =============================================================================
## find the parameter along the line to the closest point to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def __closest_point_param_1__ ( line , point ) :
    """Find the parameter along the line to the closest point
    >>> line  = ...
    >>> point = ...
    >>> mu = line.closestPointParam ( point )

    """
    return _GeomFun.closestPointParam ( point , line )


__closest_point_param_1__ . __doc__ += '\n' + _GeomFun.closestPointParam .__doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'closestPointParam' ) :
    cpp.Gaudi.XYZLine.closestPointParam = __closest_point_param_1__


# =============================================================================
## find the parameter along the line to the closest point to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_param_2_ ( point , line ) :
    """Find the parameter along the line to the closest point
    >>> point = ...
    >>> line  = ...
    >>> mu = point.closestPointParam ( line )
    """
    return _GeomFun.closestPointParam ( point , line )

_closest_point_param_2_ . __doc__ += '\n' + _GeomFun.closestPointParam .__doc__

if not hasattr ( cpp.Gaudi.XYZPoint , 'closestPointParam' ) :
    cpp.Gaudi.XYZPoint.closestPointParam = _closest_point_param_2_


# =============================================================================
## check if two lines are parallel
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _parallel_lines_ ( line , line1 ) :
    """Check if two lines are parallel:
    >>> line  = ...
    >>> line1 = ...
    >>> par   = line.parallel ( line1 )
    """
    _flag = _GeomFun.parallel ( line , line1 )
    if not _flag : return False
    return True

_parallel_lines_ . __doc__ += '\n' + _GeomFun.parallel . __doc__

if not hasattr ( cpp.Gaudi.XYZLine , 'parallel' ) :
    cpp.Gaudi.XYZLine.parallel = _parallel_lines_

# =============================================================================
## helper function/wrapper for Gaudi::Math:FitMass
#  @see Gaudi::Math::FitMass
#  @see Gaudi::Math::FitMass::fit
#  @see Gaudi::Math::ParticleParams
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @param  particle (INPUT) "particle"
#  @param  mass     (INPUT) the nominal mass
#  @return the tuple (fitter,chi2)
#  @date 2009-10-22
def _fit_mass_ ( particle , mass )  :
    """Helper function/wrapper for Gaudi::Math::MassFit
    >>> particle  =  ...   ## get Gaudi::Math::ParticleParams object
    >>> mass = 5.279 * GeV ## get the mass
    >>> result, chi2 = particle.fitMass ( particle , mass )
    >>> print 'result/chi2:', result, chi2
    """
    _fitted = cpp.Gaudi.Math.ParticleParams()
    _chi2   = cpp.Gaudi.Math.FitMass.fit ( particle  ,
                                           mass      ,
                                       _fitted   )
    return (_fitted,_chi2)

cpp.Gaudi.Math.ParticleParams.fitMass = _fit_mass_


# =============================================================================
## decorate some basic vectors
for t in ( 'int'                ,
           ##'long'               ,
           ##'long long'          ,
           ##'unsigned int'       ,
           ##'unsigned long'      ,
           ##'unsigned long long' ,
           ##'float'
           'double'               ) :
    v = std.vector( t )
    v.asList   = lambda s :       [ i for i in s ]   ## convert vector into list
    v.toList   = v.asList
    v.__repr__ = lambda s : str ( [ i for i in s ] ) ## print it !
    v.__str__  = lambda s : str ( [ i for i in s ] ) ## print it !

# =============================================================================
LHCb  = cpp.LHCb
# =============================================================================
cpp.Gaudi.Math.round                 = cpp.LHCb.Math.round
cpp.Gaudi.Math.equal_to_int          = cpp.LHCb.Math.equal_to_int
cpp.Gaudi.Math.equal_to_uint         = cpp.LHCb.Math.equal_to_uint
cpp.Gaudi.Math.isint                 = cpp.LHCb.Math.isint
cpp.Gaudi.Math.islong                = cpp.LHCb.Math.islong
cpp.Gaudi.Math.knuth_equal_to_double = cpp.LHCb.Math.knuth_equal_to_double
cpp.Gaudi.Math.round_N               = cpp.LHCb.Math.round_N
cpp.Gaudi.Math.frexp10               = cpp.LHCb.Math.frexp10

cpp.Gaudi.Math.next_double           = cpp.LHCb.Math.next_double 
cpp.Gaudi.Math.next_float            = cpp.LHCb.Math.next_float
cpp.Gaudi.Math.lomont_compare_float  = cpp.LHCb.Math.lomont_compare_float
cpp.Gaudi.Math.lomont_compare_double = cpp.LHCb.Math.lomont_compare_double


if not hasattr ( cpp.Gaudi.Math.Splines , 'DATA'    ) or \
   not hasattr ( cpp.Gaudi.Math.Splines , 'PAIR'    ) or \
   not hasattr ( cpp.Gaudi.Math.Splines , 'DATAERR' ) :

    _pair_ = std.pair('double','double')
    cpp.Gaudi.Math.Splines.DATA    = std.vector ( _pair_ )
    cpp.Gaudi.Math.Splines.PAIR    = std.pair   ( 'double',cpp.Gaudi.Math.ValueWithError )
    cpp.Gaudi.Math.Splines.DATAERR = std.vector ( cpp.Gaudi.Math.Splines.PAIR )

# =============================================================================
## make some simple interpolation for set of data points
#  @see Gaudi::Math::Splines::interpolate
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2013-03-17
def _v_interp_ ( self , x ) :
    """Simple interpolation for set of data points.
    The vector of pairs is interpreted as vector of (x,y) pairs
    >>> vct_of_pairs = ...
    >>> result       = vct_of_pairs.interpolate ( x )
    """
    if hasattr ( x , 'value' ) : x = x.value()
    #
    ## make the interpolation
    #
    return cpp.Gaudi.Math.Splines.interpolate ( self , x )

_v_interp_ . __doc__  += '\n' + cpp.Gaudi.Math.Splines.interpolate. __doc__

cpp.Gaudi.Math.Splines.DATA    .interpolate = _v_interp_
cpp.Gaudi.Math.Splines.DATAERR .interpolate = _v_interp_

# =============================================================================
## build a spline to approximate the data poinst
#  @see Gaudi::Math::Spline
#  @see GaudiMath::Spline
#  @see GaudiMath::SplineBase
#  @see Genfun::GaudiMathImplementation::SplineBase
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2013-03-17
def _v_spline_ ( self                                      ,
                 type  = cpp.GaudiMath.Interpolation.Akima ,
                 null  = False                             ,
                 scale = 1                                 ,
                 shift = 0                                 ) :
    """Create spline object for the data vector
    >>> vdata  = ...
    >>> spline = vdata.spline ()
    >>> value  = spline ( 10 )
    """
    return cpp.Gaudi.Math.Spline ( self , type , null , scale , shift )

# =============================================================================
## build a spline to approximate the data poinst
#  @see Gaudi::Math::SplineErrors
#  @see GaudiMath::Spline
#  @see GaudiMath::SplineBase
#  @see Genfun::GaudiMathImplementation::SplineBase
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2013-03-17
def _v_spline_err_ ( self                                      ,
                     type  = cpp.GaudiMath.Interpolation.Akima ,
                     null  = False                             ,
                     scale = 1                                 ,
                     shift = 0                                 ) :
    """Create spline object for the data vector
    >>> vdata  = ...
    >>> spline = vdata.splineErr()
    >>> value  = spline ( 10 )
    """
    return cpp.Gaudi.Math.SplineErrors ( self , type , null , scale , shift )

_v_spline_     . __doc__ += '\n' + cpp.Gaudi.Math.Spline       .__init__ .__doc__
_v_spline_err_ . __doc__ += '\n' + cpp.Gaudi.Math.SplineErrors .__init__ .__doc__

cpp.Gaudi.Math.Splines.DATA    .spline     = _v_spline_
cpp.Gaudi.Math.Splines.DATAERR .spline     = _v_spline_
cpp.Gaudi.Math.Splines.DATAERR .splineErr  = _v_spline_err_

# =============================================================================
## get values from the DATAERR
def _v_get_values_ ( self ) :
    """Get values from vector
    >>> vdataerr = ...
    >>> vvalues  = vdataerr.getValues()
    """
    return cpp.Gaudi.Math.Spline.getValues ( self )

## get values from the DATAERR
def _v_get_errors_ ( self ) :
    """Get errors from vector
    >>> vdataerr = ...
    >>> verrors  = vdataerr.getErrors()
    """
    return cpp.Gaudi.Math.Spline.getErrors ( self )

_v_get_values_ . __doc__  += '\n' + cpp.Gaudi.Math.Splines.getValues. __doc__
_v_get_errors_ . __doc__  += '\n' + cpp.Gaudi.Math.Splines.getErrors. __doc__

cpp.Gaudi.Math.Splines.DATAERR .getValues = _v_get_values_
cpp.Gaudi.Math.Splines.DATAERR .getErrors = _v_get_errors_

cpp.Gaudi.Math.Splines.DATA    . __repr__ = lambda s : str ( [ ( i.first , i.second ) for i in s ] )
cpp.Gaudi.Math.Splines.DATAERR . __repr__ = lambda s : str ( [ ( i.first , i.second ) for i in s ] )
cpp.Gaudi.Math.Splines.DATA    . __str__  = lambda s : str ( [ ( i.first , i.second ) for i in s ] )
cpp.Gaudi.Math.Splines.DATAERR . __str__  = lambda s : str ( [ ( i.first , i.second ) for i in s ] )

if not hasattr ( cpp.Gaudi.Math.Spline       , 'DATA'    ) :
    cpp.Gaudi.Math.Spline.DATA          = Gaudi.Math.Splines.DATA
if not hasattr ( cpp.Gaudi.Math.Spline       , 'DATAERR' ) :
    cpp.Gaudi.Math.Spline.DATAERR       = Gaudi.Math.Splines.DATAERR
if not hasattr ( cpp.Gaudi.Math.SplineErrors , 'DATAERR' ) :
    cpp.Gaudi.Math.SplineErrors.DATAERR = Gaudi.Math.Splines.DATAERR


## self-printout of TMaxtrix 
def _tmg_str_ ( self , fmt = ' %+11.4g') :
    """Self-printout of TMatrix
    """
    _rows = self.GetNrows()
    _cols = self.GetNcols()
    _line = ''
    for _irow in range ( 0 , _rows ) :
        _line += ' |'
        for _icol in range ( 0 , _cols ) :
            _line += fmt % self( _irow , _icol )
        _line += ' |'
        if ( _rows - 1 )  != _irow : _line += '\n'
    return _line


ROOT.TMatrix.__repr__  = _tmg_str_
ROOT.TMatrix.__str__   = _tmg_str_

# =============================================================================
VE            = cpp.Gaudi.Math.ValueWithError
SE            = cpp.StatEntity 
WSE           = cpp.Gaudi.Math.WStatEntity 
# =============================================================================
# temporary trick, to be removed 
# =============================================================================

SE .__repr__ = lambda s : 'Stat: '+ s.toString()
SE .__str__  = lambda s : 'Stat: '+ s.toString()

# =============================================================================
# minor decoration for StatEntity 
# ============================================================================= 
if not hasattr ( SE , '_orig_sum'  ) : 
    _orig_sum    = SE.sum
    SE._orig_sum = _orig_sum
    
if not hasattr ( SE , '_orig_mean' ) : 
    _orig_mean    = SE.mean
    SE._orig_mean = _orig_mean
    
SE. sum     = lambda s : VE ( s._orig_sum  () , s.sum2()       )
SE. minmax  = lambda s :    ( s.flagMin()     , s.flagMax()    ) 
SE. mean    = lambda s : VE ( s._orig_mean () , s.meanErr()**2 )

# =============================================================================
# minor decoration for WStatEntity 
# ============================================================================= 
if not hasattr ( WSE , '_orig_sum'  ) : 
    _orig_sum     = WSE.sum
    WSE._orig_sum = _orig_sum

if not hasattr ( WSE , '_orig_mean' ) : 
    _orig_mean_wse = WSE.mean
    WSE._orig_mean = _orig_mean_wse
    
WSE. sum     = lambda s : VE ( s._orig_sum  () , s.sum2()       )
WSE. mean    = lambda s : VE ( s._orig_mean () , s.meanErr()**2 )
WSE. minmax  = lambda s :            s.values  ().minmax() 
WSE.__repr__ = lambda s : 'WStat: '+ s.toString()
WSE.__str__  = lambda s : 'WStat: '+ s.toString()


# =============================================================================
## get the B/S estimate from the formula 
#  \f$ \sigma  = \fras{1}{S}\sqrt{1+\frac{B}{S}}\f$
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2012-10-15
def _b2s_ ( s )  :
    """Get B/S estimate from the equation:
    error(S) = 1/sqrt(S) * sqrt ( 1 + B/S)
    >>> v = ...
    >>> b2s = v.b2s() ## get B/S estimate
    """
    #
    c2 = s.cov2  ()
    #
    if s.value() <= 0  or c2 <= 0 : return VE(-1,0) 
    #
    return c2/s - 1 

# =============================================================================
## get the precision with some  error estimation 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2012-10-15
def _prec2_ ( s )  :
    """Get precision with ``some'' error estimate 
    >>> v = ...
    >>> p = v.prec ()     
    """
    if not hasattr ( s , 'value' ) :
        return _prec_ ( VE ( s , 0 ) )
    #
    c =       s.error ()
    #
    if     c <  0 or s.value() == 0  : return VE(-1,0)
    elif   c == 0                    : return VE( 0,0)
    #
    return c/abs(s) 


VE . b2s        = _b2s_
VE . prec       = _prec2_
VE . precision  = _prec2_

# =============================================================================
## add something to std::vector 
def _add_to ( vct , arg1 , *args ) :
    ##
    if hasattr ( arg1 , '__iter__' ) :
        for a in arg1 : vct.push_back ( a ) 
    else : vct.push_back ( arg1 ) 
    #
    for a in args : _add_to ( vct , a )
        
# =============================================================================
## construct std::vector<double> from the arguments
def doubles ( arg1 , *args ) :
    """Construct the std::vector<double> from the arguments
    >>> v1 = doubles ( 1.01 )
    >>> v2 = doubles ( 1.01 , 1.02 , 1.03  )
    >>> v3 = doubles ( [ 1.01 , 1.02 , 1.03 ] )    
    """
    ## create new vector 
    VT  = std.vector('double')
    vct = VT()
    ## add arguments to the vector 
    _add_to ( vct , arg1 , *args )
    ## 
    return vct

# =============================================================================
## construct std::vector<ints> from the arguments
def ints ( arg1 , *args ) :
    """Construct the std::vector<int> from the arguments    
    >>> v1 = ints ( 1 )
    >>> v2 = ints ( 1 , 1 , 1  )
    >>> v3 = ints ( [ 1 , 2 , 3 ] )    
    """
    ## create new vector 
    VT  = std.vector('int')
    vct = VT()
    ## add arguments to the vector 
    _add_to ( vct , arg1 , *args )
    ## 
    return vct

# =============================================================================
## construct std::vector<unsigned int> from the arguments
def uints ( arg1 , *args ) :
    """Construct the std::vector<unsigned int> from the arguments    
    >>> v1 = uints ( 1 )
    >>> v2 = uints ( 1 , 1 , 1  )
    >>> v3 = uints ( [ 1 , 2 , 3 ] )    
    """
    ## create new vector 
    VT  = std.vector('unsigned int')
    vct = VT()
    ## add arguments to the vector 
    _add_to ( vct , arg1 , *args )
    ## 
    return vct

# =============================================================================
## construct std::vector<long> from the arguments
def longs ( arg1 , *args ) :
    """Construct the std::vector<long> from the arguments    
    >>> v1 = longs ( 1 )
    >>> v2 = longs ( 1 , 1 , 1  )
    >>> v3 = longs ( [ 1 , 2 , 3 ] )    
    """
    ## create new vector 
    VT  = std.vector('long')
    vct = VT()
    ## add arguments to the vector 
    _add_to ( vct , arg1 , *args )
    ## 
    return vct

# =============================================================================
## construct std::vector<unsigned long> from the arguments
def ulongs ( arg1 , *args ) :
    """Construct the std::vector<unsigned long> from the arguments    
    >>> v1 = ulongs ( 1 )
    >>> v2 = ulongs ( 1 , 1 , 1  )
    >>> v3 = ulongs ( [ 1 , 2 , 3 ] )    
    """
    ## create new vector 
    VT  = std.vector('unsigned long')
    vct = VT()
    ## add arguments to the vector 
    _add_to ( vct , arg1 , *args )
    ## 
    return vct

SPD = std.pair('double','double')
SPD.asTuple  = lambda s : (s.first,s.second)
SPD.__str__  = lambda s : str( (s.first,s.second) )
SPD.__repr__ = SPD.__str__

VE = cpp.Gaudi.Math.ValueWithError

_is_equal_ = cpp.LHCb.Math.Equal_To    ( 'double' )()
_is_zero_  = cpp.LHCb.Math.Zero        ( 'double' )()
_is_le_    = cpp.LHCb.Math.LessOrEqual ( 'double' )()

# =============================================================================
## Comparison of ValueWithError object with other objects
#  @attention it is comparison by value only, errors are ignored 
def _ve_lt_ ( self , other ) :
    """Comparison of ValueWithError object with other objects
    >>> a = VE( ... )
    >>> print a < b 
    Attention: comparison by value only!
    """
    return float(self) < float(other)

# =============================================================================
## Comparison of ValueWithError object with other objects
#  @attention it is comparison by value only, errors are ignored 
def _ve_le_ ( self , other ) :
    """Comparison of ValueWithError object with other objects
    >>> a = VE( ... )
    >>> print a <= b 
    Attention: comparison by value only!
    """
    return _is_le_ ( float(self) , float(other) )

# =============================================================================
## Comparison of ValueWithError object with other objects
#  @attention it is comparison by value only, errors are ignored 
def _ve_gt_ ( self , other ) :
    """Comparison of ValueWithError object with other objects
    >>> a = VE( ... )
    >>> print a > b 
    Attention: comparison by value only!
    """
    return float(self) > float(other)

# =============================================================================
## Comparison of ValueWithError object with other objects
#  @attention it is comparison by value only, errors are ignored 
def _ve_ge_ ( self , other ) :
    """Comparison of ValueWithError object with other objects
    >>> a = VE( ... )
    >>> print a >= b 
    Attention: comparison by value only!
    """
    return _is_le_ ( float(other) , float(self) )
    
VE.__lt__ = _ve_lt_ 
VE.__le__ = _ve_le_ 
VE.__gt__ = _ve_gt_ 
VE.__ge__ = _ve_ge_ 

# =============================================================================
## Equality for ValueWithError objects
def _ve_eq_ ( self , other ) :
    """Equality for ValueWithError objects
    >>> a = VE( ... )
    >>> b = VE( ... )
    >>> print a == b 
    """
    if isinstance ( other , VE ) :
        v1 = self .value()
        v2 = other.value()
        return _is_equal_ ( v1 , v2 ) and _is_equal_ ( self.cov2() , other.cov2() )
    elif _is_zero_ ( self.cov2() )  :
        return _is_equal_ ( float ( self ) ,  float ( other )  ) 
    else :
        raise NotImplementedError,' Equality for %s and  %s is not implemented' % ( self, other) 

# =============================================================================
## inequality for ValueWithError objects
def _ve_ne_ ( self , other ) :
    """Inequality for ValueWithError objects
    >>> a = VE( ... )
    >>> b = VE( ... )
    >>> print a != b 
    """
    try: 
        return not self == other
    except NotImplemented,s :
        raise NotImplementedError,' Inequality for %s and  %s is not implemented' % ( self, other) 

VE . __eq__ = _ve_eq_
VE . __ne__ = _ve_ne_


# =============================================================================
# Imporve operations with std.complex 
# =============================================================================
COMPLEX = cpp.std.complex('double')

def _cmplx_to_complex_ ( s ) :
    """convert to complex"""
    return  complex    ( s.real() , s.imag() )
def _cmplx_negate_     ( s ) :
    """Negation:
    >>> v  = ...
    >>> v1 = -v
    """
    return -complex    ( s.real() , s.imag() )
def _cmplx_abs_        ( s ) :
    """Absolute value
    >>> print abs(v) 
    """
    import math
    sr = s.real()
    si = s.imag()
    return math.sqrt( sr * sr + si * si ) 
def _cmplx_norm_       ( s ) :
    """Norm (squared absolute value)
    >>> print v.norm()
    """
    sr = s.real()
    si = s.imag()
    return sr * sr + si * si
def _cmplx_conjugate_  ( s ) :
    """Get complex conjugated
    >>> vc = v.conjugate() 
    """
    return complex     ( s.real() , -s.imag() )
    
def _cmplx_add_        ( s , o ) :
    """add complex values 
    >>> r = v + other  
    """
    return o + complex ( s.real() , s.imag() )
def _cmplx_mul_        ( s , o ) :
    """multiply  complex values 
    >>> r = v * other  
    """
    return o * complex ( s.real() , s.imag() )

def _cmplx_div_        ( s , o ) :
    return (1.0/o) * complex ( s.real() , s.imag() )
    """divide complex values 
    >>> r = v / other  
    """
def _cmplx_rdiv_       ( s , o ) :
    """divide complex values 
    >>> r = other / v 
    """
    return o       * ( 1.0 / complex ( s.real() , s.imag() ) )

def _cmplx_sub_        ( s , o ) :
    """subtract complex values 
    >>> r = v - other 
    """
    return (-o   ) + complex ( s.real() , s.imag() )
def _cmplx_rsub_       ( s , o ) :
    """subtract complex values 
    >>> r = other - v 
    """
    return   o     - complex ( s.real() , s.imag() )

def _cmplx_pow_  ( s , o ) :
    """power function 
    >>> r = v ** other  
    """
    if isinstance ( o , COMPLEX ) :
        o = complex ( o.real() , o.imag() ) 
    return complex ( s.real() , s.imag() ) ** o

def _cmplx_rpow_  ( s , o ) :
    """power function 
    >>> r = other **v  
    """
    return o ** complex ( s.real() , s.imag() )


def _cmplx_eq_    ( s , o ) :
    """equality:
    >>> r = v == other  
    """
    if isinstance ( o, COMPLEX ) :
        return s.real() == o.real() and s.imag() == o.imag()
    return complex( s.real() , s.imag() ) == o

def _cmplx_ne_    ( s , o ) :
    """non-equality:
    >>> r = v != other  
    """
    if isinstance ( o, COMPLEX ) :
        return s.real() != o.real() or  s.imag() != o.imag()
    return complex( s.real() , s.imag() ) != o 
    
COMPLEX.__complex__ = _cmplx_to_complex_

COMPLEX.__add__     = _cmplx_add_
COMPLEX.__mul__     = _cmplx_mul_
COMPLEX.__div__     = _cmplx_div_
COMPLEX.__sub__     = _cmplx_sub_

COMPLEX.__radd__    = _cmplx_add_
COMPLEX.__rmul__    = _cmplx_mul_
COMPLEX.__rdiv__    = _cmplx_rdiv_
COMPLEX.__rsub__    = _cmplx_rsub_

def _cmplx_iadd_ ( s , o ) :
    x = s + o
    s.real(x.real)
    s.imag(x.imag)
    
def _cmplx_isub_ ( s , o ) :
    x = s - o
    s.real(x.real)
    s.imag(x.imag)

def _cmplx_imul_ ( s , o ) :
    x = s * o
    s.real(x.real)
    s.imag(x.imag)

def _cmplx_idiv_ ( s , o ) :
    x = s / o
    s.real(x.real)
    s.imag(x.imag)

COMPLEX.__iadd__    = _cmplx_iadd_
COMPLEX.__imul__    = _cmplx_imul_
COMPLEX.__idiv__    = _cmplx_idiv_
COMPLEX.__isub__    = _cmplx_isub_

COMPLEX.__repr__    = lambda s : "%s" % complex ( s.real(), s.imag() )
COMPLEX.__str__     = lambda s : "%s" % complex ( s.real(), s.imag() )
COMPLEX.__abs__     = _cmplx_abs_
COMPLEX.__pow__     = _cmplx_pow_
COMPLEX.__rpow__    = _cmplx_rpow_
COMPLEX.__neg__     =  _cmplx_negate_

COMPLEX.__eq__      =  _cmplx_eq_
COMPLEX.__ne__      =  _cmplx_ne_

COMPLEX.norm        = _cmplx_norm_
COMPLEX.conjugate   = _cmplx_conjugate_
COMPLEX.conj        = _cmplx_conjugate_
COMPLEX.to_complex  = _cmplx_to_complex_ 
COMPLEX.as_complex  = _cmplx_to_complex_ 


# =============================================================================
if '__main__' == __name__ :

    ## make printout of the own documentations
    print '*'*120
    print                      __doc__
    print ' Author  : %s ' %   __author__
    print ' Version : %s ' %   __version__
    print ' Date    : %s ' %   __date__
    print '*'*120

    _v = [ l for l in dir(Gaudi     ) if 0 != l.find('__') ]
    print ' dir(Gaudi)      : '
    _v.sort()
    for v in _v : print v
    _v = [ l for l in dir(Gaudi.Math) if 0 != l.find('__') ]
    print ' dir(Gaudi.Math) : '
    _v.sort()
    for v in _v : print v


# =============================================================================
# The  END
# =============================================================================
