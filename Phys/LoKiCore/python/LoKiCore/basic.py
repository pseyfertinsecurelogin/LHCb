#!/usr/bin/env python
# =============================================================================
# $Id: basic.py 166436 2014-01-06 11:41:17Z marcocle $
# =============================================================================
# $URL: http://svn.cern.ch/guest/lhcb/LHCb/tags/Phys/LoKiCore/v11r17/python/LoKiCore/basic.py $
# =============================================================================
## @file decorators.py LoKiCore/basic.py
#
#  The set of basic decorator for objects from LoKiCore library
#
#        This file is a part of LoKi project -
#    "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#
#  $Revision: 166436 $
#  Last modification $Date: 2014-01-06 12:41:17 +0100 (Mon, 06 Jan 2014) $
#                 by $Author: marcocle $
# =============================================================================
"""
The set of basic decorators for objects from LoKiCore library

    This file is a part of LoKi project -
``C++ ToolKit  for Smart and Friendly Physics Analysis''

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.

"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__date__    = "2010-09-27"
__version__ = "SVN $Revision: 166436 $ "
# =============================================================================
__all__     = (
    'cpp'   ,
    'std'   ,
    'LoKi'  ,
    'LHCb'  ,
    'Gaudi'
    )
# =============================================================================

try:
    import cppyy
except ImportError:
    # FIXME: backward compatibility
    print "# WARNING: using PyCintex as cppyy implementation"
    import PyCintex as cppyy
    import sys
    sys.modules['cppyy'] = cppyy

# namespaces shortcuts
cpp   = cppyy.gbl
std   = cppyy.gbl.std
LoKi  = cppyy.gbl.LoKi
LHCb  = cppyy.gbl.LHCb
Gaudi = cppyy.gbl.Gaudi

# =============================================================================
if '__main__' == __name__ :

    print 80*'*'
    print __doc__
    print ' Author  : ' , __author__
    print ' Version : ' , __version__
    print ' Date    : ' , __date__
    print 80*'*'
    for i in dir() : print i


# =============================================================================
# The END
# =============================================================================
