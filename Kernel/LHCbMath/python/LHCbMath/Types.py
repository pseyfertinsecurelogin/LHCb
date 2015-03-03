#!/usr/bin/env python
# =============================================================================
# $Id: Types.py 101260 2010-11-14 18:02:25Z ibelyaev $
# =============================================================================
# $URL: http://svn.cern.ch/guest/lhcb/LHCb/tags/Kernel/LHCbMath/v3r22/python/LHCbMath/Types.py $
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
#  Last modification $Date: 2010-11-14 19:02:25 +0100 (Sun, 14 Nov 2010) $
#                 by $Author: ibelyaev $
#
#
# =============================================================================
"""
Simple file to provide 'easy' access in python for the basic ROOT::Math classes

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
  
  Last modification $Date: 2010-11-14 19:02:25 +0100 (Sun, 14 Nov 2010) $
                 by $Author: ibelyaev $

"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__date__    = "2009-09-12"
__version__ = "Version$Revision: 101260 $"
# =============================================================================
__all__     = () ## nothing to be imported !
# =============================================================================
import ROOT

## get global C++ namespace 
from GaudiPython.Bindings import gbl as cpp

## C++ namespace Gaudi
Gaudi = cpp.Gaudi
std   = cpp.std

## ROOT::Math namespace 
_RM = ROOT.ROOT.Math


Gaudi.XYZPoint            = _RM.PositionVector3D     ('ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag')
Gaudi.XYZVector           = _RM.DisplacementVector3D ('ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag')
Gaudi.LorentzVector       = _RM.LorentzVector        ('ROOT::Math::PxPyPzE4D<double>')
Gaudi.Plane3D             = _RM.Plane3D

Gaudi.Math.XYZPoint       = Gaudi.XYZPoint
Gaudi.Math.XYZVector      = Gaudi.XYZVector
Gaudi.Math.LorentzVector  = Gaudi.LorentzVector    
Gaudi.Math.Plane3D        = Gaudi.Plane3D  

Gaudi.Point3D             = Gaudi.XYZPoint
Gaudi.Math.Point3D        = Gaudi.XYZPoint

Gaudi.Vector3D            = Gaudi.XYZVector
Gaudi.Math.Vector3D       = Gaudi.XYZVector

Gaudi.Vector2             = _RM.SVector( 'double' , 2 )
Gaudi.Vector3             = _RM.SVector( 'double' , 3 )
Gaudi.Vector4             = _RM.SVector( 'double' , 4 )
Gaudi.Vector5             = _RM.SVector( 'double' , 5 )
Gaudi.Vector8             = _RM.SVector( 'double' , 8 )

Gaudi.Math.Vector2        = Gaudi.Vector2 
Gaudi.Math.Vector3        = Gaudi.Vector3 
Gaudi.Math.Vector4        = Gaudi.Vector4 
Gaudi.Math.Vector5        = Gaudi.Vector5 
Gaudi.Math.Vector8        = Gaudi.Vector8 

Gaudi.SymMatrix2x2        = _RM.SMatrix('double,2,2,ROOT::Math::MatRepSym<double,2>')
Gaudi.SymMatrix3x3        = _RM.SMatrix('double,3,3,ROOT::Math::MatRepSym<double,3>')    
Gaudi.SymMatrix4x4        = _RM.SMatrix('double,4,4,ROOT::Math::MatRepSym<double,4>')
Gaudi.SymMatrix5x5        = _RM.SMatrix('double,5,5,ROOT::Math::MatRepSym<double,5>')
Gaudi.SymMatrix6x6        = _RM.SMatrix('double,6,6,ROOT::Math::MatRepSym<double,6>')
## LHCb::Particle
Gaudi.SymMatrix7x7        = _RM.SMatrix('double,7,7,ROOT::Math::MatRepSym<double,7>')
## Gaudi::Math::ParticleParams 
Gaudi.SymMatrix8x8        = _RM.SMatrix('double,8,8,ROOT::Math::MatRepSym<double,8>')
## LHCb:TwoProngVertex
Gaudi.SymMatrix9x9        = _RM.SMatrix('double,9,9,ROOT::Math::MatRepSym<double,9>')


Gaudi.Math.SymMatrix3x3   = Gaudi.SymMatrix2x2 
Gaudi.Math.SymMatrix3x3   = Gaudi.SymMatrix3x3 
Gaudi.Math.SymMatrix4x4   = Gaudi.SymMatrix4x4
Gaudi.Math.SymMatrix5x5   = Gaudi.SymMatrix5x5 
Gaudi.Math.SymMatrix6x6   = Gaudi.SymMatrix6x6 
Gaudi.Math.SymMatrix7x7   = Gaudi.SymMatrix7x7 
Gaudi.Math.SymMatrix8x8   = Gaudi.SymMatrix8x8 
Gaudi.Math.SymMatrix9x9   = Gaudi.SymMatrix9x9

#
# specific matrices for 'tracks'
#

Gaudi.Matrix5x5             = _RM.SMatrix('double,5,5')
Gaudi.TrackMatrix           = Gaudi.Matrix5x5 
Gaudi.Math.Matrix5x5        = Gaudi.Matrix5x5 
Gaudi.Math.TrackMatrix      = Gaudi.TrackMatrix 

Gaudi.TrackSymMatrix        = Gaudi.SymMatrix5x5
Gaudi.Math.TrackSymMatrix   = Gaudi.TrackSymMatrix 

Gaudi.TrackVector           = Gaudi.Vector5 
Gaudi.Math.TrackVector      = Gaudi.TrackVector

#
# matrix from LHCb::Particle
#
Gaudi.Matrix4x3             = _RM.SMatrix('double,4,3')
Gaudi.Math.Matrix4x3        = Gaudi.Matrix4x3

    
## Gaudi::Math
Gaudi.Math.XYZLine           = cpp.Gaudi.Math.Line(Gaudi.XYZPoint,Gaudi.XYZVector)
Gaudi.Math.SVector2WithError = cpp.Gaudi.Math.SVectorWithError(2,'double')
Gaudi.Math.SVector3WithError = cpp.Gaudi.Math.SVectorWithError(3,'double')
Gaudi.Math.SVector4WithError = cpp.Gaudi.Math.SVectorWithError(4,'double')
Gaudi.Math.SVector5WithError = cpp.Gaudi.Math.SVectorWithError(5,'double')
Gaudi.Math.SVector8WithError = cpp.Gaudi.Math.SVectorWithError(8,'double')

Gaudi.XYZLine                = Gaudi.Math.XYZLine
Gaudi.Line3D                 = Gaudi.Math.XYZLine
Gaudi.Math.Line3D            = Gaudi.Math.XYZLine


Gaudi.Math.frac              = Gaudi.Math.Functions.frac
Gaudi.Math.asym              = Gaudi.Math.Functions.asym
Gaudi.Math.binomEff          = Gaudi.Math.Functions.binomEff 


## vetcors of vectors
Gaudi.Vectors2       = std.vector( Gaudi.Vector2 )
Gaudi.Vectors3       = std.vector( Gaudi.Vector3 )
Gaudi.Vectors4       = std.vector( Gaudi.Vector4 )
Gaudi.Math.Vectors2  = Gaudi.Vectors2 
Gaudi.Math.Vectors3  = Gaudi.Vectors3
Gaudi.Math.Vectors4  = Gaudi.Vectors4 

## ============================================================================
## some useful decoration:
## ============================================================================

_P3D = Gaudi.XYZPoint
_V3D = Gaudi.XYZVector
_V4D = Gaudi.LorentzVector

def _o_mul_ ( self , other ) :
    _typ  = self.__class__
    tmp   = _typ ( self )
    tmp  *= other 
    return tmp

def _o_add_ ( self , other ) :
    _typ  = self.__class__
    tmp   = _typ ( self )
    tmp  += other 
    return tmp

def _o_sub_ ( self , other ) :
    _typ  = self.__class__
    tmp   = _typ ( self )
    tmp  -= other 
    return tmp

## Self-printout of 3D-points and 3D-vectors
def _o1_str_ ( self , fmt = "( %g, %g, %g) ") :
    """    
    Self-printout of 3D-points and 3D-vectors
    
    """
    return fmt % ( self.X() , self.Y( ), self.Z() )

## Self-printout of 4D-vectors 
def _o2_str_ ( self , fmt = "[( %g, %g, %g), %g]" ) :
    """    
    Self-printout of 4D-vectors 
    
    """
    return fmt % ( self.X() , self.Y( ), self.Z() , self.E() )


_P3D.__mul__ = _o_mul_
_V3D.__mul__ = _o_mul_
_V4D.__mul__ = _o_mul_

_P3D.__add__ = _o_add_
_V3D.__add__ = _o_add_
_V4D.__add__ = _o_add_

_P3D.__sub__ = _o_sub_
_V3D.__sub__ = _o_sub_
_V4D.__sub__ = _o_sub_
    
_V3D. __rmul__ = _V3D.__mul__ 
_V4D. __rmul__ = _V4D.__mul__

if not hasattr ( _P3D , '_new_str_' ) :
    _P3D._new_str_ = _o1_str_  
    _P3D.__str__   = _o1_str_
    _P3D.__repr__  = _o1_str_

if not hasattr ( _V3D , '_new_str_' ) :
    _V3D._new_str_ = _o1_str_
    _V3D.__str__   = _o1_str_
    _V3D.__repr__  = _o1_str_
    
if not hasattr ( _V4D , '_new_str_' ) :    
    _V4D._new_str_ = _o2_str_
    _V4D.__str__   = _o2_str_
    _V4D.__repr__  = _o2_str_

# =============================================================================
## Self-printout of line
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _l_str_ ( self ) :
    """
    Self-printout of line: (point, direction)
    """
    return "(%s,%s)" % ( self.beginPoint() , self.direction() )

if not hasattr ( Gaudi.Math.XYZLine , '_new_str_' ) :
    Gaudi.Math.XYZLine._new_str_ = _l_str_
    Gaudi.Math.XYZLine.__str__   = _l_str_
    Gaudi.Math.XYZLine.__repr__  = _l_str_

# ============================================================================
## self-printout of matrices
def _mg_str_ ( self , fmt = ' %+11.4g') :
    """
    Self-printout of matrices 
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
    """
    Self-printout of symetrical matrices 
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

## get the correlation matrix
def _m_corr_ ( self ) :
    """
    Get the correlation matrix
    
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
    
    
for m in ( Gaudi.Matrix5x5      ,
           Gaudi.TrackMatrix    ,
           Gaudi.Matrix4x3      ) :
    if not hasattr ( m , '_new_str_' ) : 
        m. _new_str_ = _mg_str_
        m. __repr__  = _mg_str_
        m. __str__   = _mg_str_
    
for m in ( Gaudi.SymMatrix2x2   ,
           Gaudi.SymMatrix3x3   ,
           Gaudi.SymMatrix4x4   ,
           Gaudi.SymMatrix5x5   ,
           Gaudi.SymMatrix6x6   ,
           Gaudi.SymMatrix7x7   ,
           Gaudi.SymMatrix8x8   ,
           Gaudi.SymMatrix9x9   ,
           Gaudi.TrackSymMatrix ) :
    
    if not hasattr ( m , 'correlations' ) :
        m.correlations = _m_corr_
        
    if not hasattr ( m , '_new_str_' ) : 
        m. _new_str_ = _ms_str_
        m. __repr__  = _ms_str_
        m. __str__   = _ms_str_
        
                      
# =============================================================================
## Self-printout of 3D-plane
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _p_str_ ( self ) :
    """
    Self-printout of 3D-plane: (point, normal)
    """
    return "(%s,%s)" % ( self.ProjectOntoPlane( Gaudi.XYZPoint()) , self.Normal() )

if not hasattr ( Gaudi.Plane3D , '_new_str_' ) :
    Gaudi.Plane3D._new_str_ = _p_str_
    Gaudi.Plane3D.__str__   = _p_str_
    Gaudi.Plane3D.__repr__  = _p_str_

# =============================================================================
## self-printout of S-vectors 
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _v_str_ ( self , fmt = ' %g' ) :
    """
    Self-printout of SVectors: (...)
    """
    index  = 0
    result = ''
    while index < self.kSize :
        if 0 != index : result += ', '
        result += fmt % self.At( index )
        index  += 1 
    return "( " + result + ' )'

for t in  ( Gaudi.Vector2 ,
            Gaudi.Vector3 ,
            Gaudi.Vector4 ,
            Gaudi.Vector5 ) :
    if not hasattr ( t , '_new_str_' ) :
        t._new_str_ = _v_str_
        t.__str__   = _v_str_
        t.__repr__  = _v_str_
        
for t in ( Gaudi.Math.ValueWithError         ,
           Gaudi.Math.Point3DWithError       ,
           Gaudi.Math.Vector3DWithError      ,
           Gaudi.Math.LorentzVectorWithError ,
           Gaudi.Math.SVector3WithError      , 
           Gaudi.Math.SVector4WithError      , 
           Gaudi.Math.SVector5WithError      , 
           Gaudi.Math.SVector8WithError      ) :
    if not hasattr ( t , '_new_str_' ) :
        t._new_str_ = t.toString
        t.__str__   = t.toString
        t.__repr__  = t.toString

        
## get the eigenvalues for symmetric matrices :
def _eigen_1_ ( self , sorted = True ) :
    """
    
    >>> mtrx = ...
    >>> values = mtrx.eigenValues ( sorted = True )
    
    """
    return Gaudi.Math.EigenSystems.eigenValues ( self , sorted )


## get the eigevectors for symmetric matrices :
def _eigen_2_ ( self , sorted = True ) :
    """
    
    >>> mtrx = ...
    >>> values, vectors = mtrx.eigenVectors( sorted = True )
    
    """
    if   2 == mtrx.kCols :
        _values  = Gaudi.Vector2  ()
        _vectors = Gaudi.Vectors2 ()
    elif 3 == mtrx.kCols :
        _values  = Gaudi.Vector3  ()
        _vectors = Gaudi.Vectors3 ()
    elif 4 == mtrx.kCols :
        _values  = Gaudi.Vector4  ()
        _vectors = Gaudi.Vectors4 ()
    else :
        raise AttributeError, "Not implemented for dimention: %s" % mtrx.kCols
    
    st = Gaudi.Math.EigenSystems.eigenVectors ( self , _values , _vectors , sorted )
    if st.isFailure () :
        print 'EigenVectors: Failure from EigenSystems' , st
        
    return ( _values , _vectors )

    
_eigen_1_ .__doc__ += '\n' +  Gaudi.Math.EigenSystems.eigenValues  . __doc__
_eigen_2_ .__doc__ += '\n' +  Gaudi.Math.EigenSystems.eigenVectors . __doc__

for m in ( Gaudi.SymMatrix2x2   ,
           Gaudi.SymMatrix3x3   ,
           Gaudi.SymMatrix4x4   ) :
    if not hasattr ( m , 'eigenValues'  ) : m.eigenValues  = _eigen_1_
    if not hasattr ( m , 'eigenVectors' ) : m.eigenVectors = _eigen_2_
        

# =============================================================================
## self-printout of Gaudi::Math::ParticleParams  
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2010-05-26
def _pp_str_ ( self ) :
    """
    Self-printout of ParticleParams
    """
    pos = self.position    ()
    mom = self.momentum    ()
    len = self.decayLength () 
    return " ( %s/%s/%s ) " % ( pos, mom , len )

if not hasattr ( Gaudi.Math.ParticleParams , '_new_str_' ) :
    Gaudi.Math.ParticleParams._new_str_ = _pp_str_
    Gaudi.Math.ParticleParams.__str__   = _pp_str_
    Gaudi.Math.ParticleParams.__repr__  = _pp_str_

# =============================================================================
## various decorators for GeomFun.h
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
if not hasattr ( Gaudi.Math , 'XYZGeomFun' ) :
    Gaudi.Math.XYZGeomFun = cpp.Gaudi.Math.GF(
        Gaudi.XYZPoint  ,
        Gaudi.XYZLine   ,
        Gaudi.Plane3D 
        )
if not hasattr ( Gaudi , 'XYZGeomFun' ) :
    Gaudi.XYZGeomFun = Gaudi.Math.XYZGeomFun
    
_GeomFun = Gaudi.Math.XYZGeomFun

# =============================================================================
## intersection of line and plane 
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_line_and_plane_ ( line , plane ) :
    """
    Find the intersection of line and plane

    >>> line  = ...
    >>> plane = ...
    
    >>> ok, point, mu = line.intersect ( plane )

    The return value is a tuple: 
    - the point
    - the parameter along the line
    - the flag (true if intersection exists)
    
    """
    _point = Gaudi.XYZPoint(0,0,-1.e+10)
    _mu    = ROOT.Double(-1.e+10)
    _flag  = _GeomFun.intersection ( line   ,
                                     plane  ,
                                     _point ,
                                     _mu    )
    if _flag : _flag = True
    else     : _flag = False 
    return (_point,_mu,_flag)
    
_intersect_line_and_plane_ . __doc__ += '\n' + _GeomFun.intersection . __doc__ 

if not hasattr ( Gaudi.XYZLine , 'intersect' ) :
    Gaudi.XYZLine.intersect = _intersect_line_and_plane_

# =============================================================================
## intersect two planes 
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_two_planes_ ( plane , plane1 ) :
    """
    Find the intersection line for two planes:

    >>> plane  = ...
    >>> plane1 = ...
    >>> line, flag = plane.line(plane1)

    Return value is a tuple:
    
    - the intersection line
    - the flag (true if intersection exists)
    
    """
    _line = Gaudi.XYZLine()
    _flag = _GeomFun.intersection ( plane , plane1 , _line )
    if _flag : _flag = True
    else     : _flag = False 
    return (_line,_flag)

_intersect_two_planes_ . __doc__ += '\n' + _GeomFun.intersection . __doc__ 

if not hasattr ( Gaudi.Plane3D , 'line' ) :
    Gaudi.Plane3D.line = _intersect_two_planes_


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
    _point = Gaudi.XYZPoint(0,0,-1.e+10)
    _flag = _GeomFun.intersection ( plane , plane1 , plane2 , _point )
    if _flag : _flag = True
    else     : _flag = False 
    return (_point,_flag)


_intersect_three_planes_ . __doc__ += '\n' + _GeomFun.intersection . __doc__ 

if not hasattr ( Gaudi.Plane3D , 'point' ) :
    Gaudi.Plane3D.point = _intersect_three_planes_


# =============================================================================
## intersect the planes 
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _intersect_the_planes_ ( plane , plane1 , plane2 = None ) :
    """
    Find the intersection line/point for two or three planes:

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

if not hasattr ( Gaudi.Plane3D , 'intersect' ) :
    Gaudi.Plane3D.intersect = _intersect_the_planes_


# =============================================================================
## calculate the impact parameter of the line & point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _imp_par_1_ ( line , point ) :
    """
    Calculate the impact parameter of the line and the point

    >>> line  = ...
    >>> point = ...
    >>> ip = line.impactParameter ( point )
    
    """
    return _GeomFun.impactParameter ( point , line )

_imp_par_1_ . __doc__ += '\n' + _GeomFun.impactParameter . __doc__ 

if not hasattr ( Gaudi.XYZLine , 'impactParameter' ) :
    Gaudi.XYZLine.impactParameter = _imp_par_1_
if not hasattr ( Gaudi.XYZLine , 'ip'              ) :
    Gaudi.XYZLine.ip              = _imp_par_1_

# =============================================================================
## calculate the impact parameter of the line & point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _imp_par_2_ ( point , line ) :
    """
    Calculate the impact parameter of the line and the point

    >>> point = ...
    >>> line  = ...
    >>> ip = point.impactParameter ( line )
    
    """
    return _GeomFun.impactParameter ( point , line )


_imp_par_2_ . __doc__ += '\n' + _GeomFun.impactParameter . __doc__ 

if not hasattr ( Gaudi.XYZPoint , 'impactParameter' ) :
    Gaudi.XYZPoint.impactParameter = _imp_par_2_
if not hasattr ( Gaudi.XYZPoint , 'ip'              ) :
    Gaudi.XYZPoint.ip              = _imp_par_2_

# =============================================================================
## distance between two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _distance_between_two_lines_ ( line , line1 ) :
    """
    Find the distance netween two lines :

    >>> line  = ...
    >>> line1 = ...
    >>> dist = line.distance ( line1 )
    
    """
    return _GeomFun.distance ( line , line1 )

_distance_between_two_lines_ . __doc__ += '\n' + _GeomFun.distance. __doc__

if not hasattr ( Gaudi.XYZLine , 'distance' ) :
    Gaudi.XYZLine.distance = _distance_between_two_lines_


# =============================================================================
## find the closest points for two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_points_ ( line , line1 ) :
    """
    Calculate the two closest points between two lines

    >>> line1 = ...
    >>> line2 = ...
    >>> point1 , point2 , flag = line1.closestPoints ( line2 )

    The return values is a tuple:
    - the point onthe fist line
    - the point on the second line
    - the flag (true is everything OK)
    
    """
    _point1 = Gaudi.XYZPoint(0,0,-1.e+10)
    _point2 = Gaudi.XYZPoint(0,0,-1.e+11)
    _flag   = _GeomFun.closestPoints ( line , line1 , _point1 , _point2 )
    if    _flag : _flag = True
    else        : _flag = False 
    return (_point1,_point2,_flag)
    

_closest_points_ . __doc__ += '\n' + _GeomFun.closestPoints . __doc__

if not hasattr ( Gaudi.XYZLine , 'closestPoints' ) :
    Gaudi.XYZLine.closestPoints = _closest_points_


# =============================================================================
## find the closest points for two lines
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_params_ ( line , line1 ) :
    """
    Calculate the parameters for two closest points between two lines

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

if not hasattr ( Gaudi.XYZLine , 'closestPointParams' ) :
    Gaudi.XYZLine.closestPointParams = _closest_point_params_

# =============================================================================
## find the point on ilne closest to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_1_ ( line , point ) :
    """
    Find the point on line closest to the given point

    >>> line  = ...
    >>> point = ...
    >>> ClosestPoint  = line.closestPoint ( point )
    
    """
    return _GeomFun.closestPoint ( point , line )

_closest_point_1_ . __doc__ += '\n' + _GeomFun.closestPoint . __doc__

if not hasattr ( Gaudi.XYZLine , 'closestPoint' ) :
    Gaudi.XYZLine.closestPoint = _closest_point_1_
    
# =============================================================================
## find the point on ilne closest to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_2_ ( point , line ) :
    """
    Find the point on line closest to the given point

    >>> point = ...
    >>> line  = ...
    >>> ClosestPoint  = point.closestPoint ( line )
    
    """
    return _GeomFun.closestPoint ( point , line )

_closest_point_2_ . __doc__ += '\n' + _GeomFun.closestPoint . __doc__

if not hasattr ( Gaudi.XYZPoint , 'closestPoint' ) :
    Gaudi.XYZPoint.closestPoint = _closest_point_2_


# =============================================================================
## find the parameter along the line to the closest point to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def __closest_point_param_1__ ( line , point ) :
    """
    Find the parameter along the line to the closest point 

    >>> line  = ...
    >>> point = ...
    >>> mu = line.closestPointParam ( point )

    """
    return _GeomFun.closestPointParam ( point , line )


__closest_point_param_1__ . __doc__ += '\n' + _GeomFun.closestPointParam .__doc__

if not hasattr ( Gaudi.XYZLine , 'closestPointParam' ) :
    Gaudi.XYZLine.closestPointParam = __closest_point_param_1__

    
# =============================================================================
## find the parameter along the line to the closest point to the given point
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _closest_point_param_2_ ( point , line ) :
    """
    Find the parameter along the line to the closest point 

    >>> point = ...
    >>> line  = ...
    >>> mu = point.closestPointParam ( line )

    """
    return _GeomFun.closestPointParam ( point , line )

_closest_point_param_2_ . __doc__ += '\n' + _GeomFun.closestPointParam .__doc__

if not hasattr ( Gaudi.XYZPoint , 'closestPointParam' ) :
    Gaudi.XYZPoint.closestPointParam = _closest_point_param_2_


# =============================================================================
## check if two lines are parallel
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-22
def _parallel_lines_ ( line , line1 ) :
    """
    Check if two lines are parallel:

    >>> line  = ...
    >>> line1 = ...
    >>> par   = line.parallel ( line1 ) 
    """
    _flag = _GeomFun.parallel ( line , line1 )
    if not _flag : return False
    return True

_parallel_lines_ . __doc__ += '\n' + _GeomFun.parallel . __doc__

if not hasattr ( Gaudi.XYZLine , 'parallel' ) :
    Gaudi.XYZLine.parallel = _parallel_lines_

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
    """
    Helper function/wrapper for Gaudi::Math::MassFit

    >>> particle  =  ...   ## get Gaudi::Math::ParticleParams object
    >>> mass = 5.279 * GeV ## get the mass
    >>> result, chi2 = particle.fitMass ( particle , mass )
    >>> print 'result/chi2:', result, chi2
    
    """
    _fitted = Gaudi.Math.ParticleParams()
    _chi2   = Gaudi.Math.FitMass.fit ( particle  ,
                                       mass      ,
                                       _fitted   )
    return (_fitted,_chi2)

Gaudi.Math.ParticleParams.fitMass = _fit_mass_

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
