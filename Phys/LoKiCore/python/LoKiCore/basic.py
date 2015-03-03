#!/usr/bin/env python
# =============================================================================
# $Id: basic.py 121023 2011-03-29 18:09:53Z ibelyaev $ 
# =============================================================================
# $URL: http://svn.cern.ch/guest/lhcb/LHCb/tags/Phys/LoKiCore/v11r6/python/LoKiCore/basic.py $ 
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
#   By usage of this code one clearly states the disagreement 
#  with the campain of Dr.O.Callot et al.: 
#  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
#
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#
#  $Revision: 121023 $
#  Last modification $Date: 2011-03-29 20:09:53 +0200 (Tue, 29 Mar 2011) $
#                 by $Author: ibelyaev $
# =============================================================================
"""
The set of basic decorators for objects from LoKiCore library

    This file is a part of LoKi project - 
``C++ ToolKit  for Smart and Friendly Physics Analysis''

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
contributions and advices from G.Raven, J.van Tilburg, 
A.Golutvin, P.Koppenburg have been used in the design.

By usage of this code one clearly states the disagreement 
with the campain of Dr.O.Callot et al.:
 ``No Vanya's lines are allowed in LHCb/Gaudi software.''


"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl" 
__date__    = "2010-09-27"
__version__ = "SVN $Revision: 121023 $ "
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
