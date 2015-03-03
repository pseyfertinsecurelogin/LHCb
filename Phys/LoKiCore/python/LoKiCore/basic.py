#!/usr/bin/env python
# =============================================================================
# $Id: basic.py 155775 2013-04-29 09:31:21Z cattanem $ 
# =============================================================================
# $URL: http://svn.cern.ch/guest/lhcb/LHCb/tags/Phys/LoKiCore/v11r12/python/LoKiCore/basic.py $ 
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
#  $Revision: 155775 $
#  Last modification $Date: 2013-04-29 11:31:21 +0200 (Mon, 29 Apr 2013) $
#                 by $Author: cattanem $
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
__version__ = "SVN $Revision: 155775 $ "
# =============================================================================
__all__     = (
    'cpp'   ,
    'std'   ,
    'LoKi'  ,
    'LHCb'  ,
    'Gaudi' 
    )
# =============================================================================

import PyCintex

# construct the global namespace 
_global   = PyCintex.makeNamespace('')

# namespaces

cpp   = _global 
std   = _global.std
LoKi  = _global.LoKi
LHCb  = _global.LHCb
Gaudi = _global.Gaudi

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
