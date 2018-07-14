#!/usr/bin/env python
# =============================================================================
## @file newparts.py
#  Simple test for adding new particle 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2018-07-12
# =============================================================================
""" Simple test for adding new particle 
"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
# =============================================================================
import PartProp.PartPropAlg
import PartProp.decorators 
import PartProp.Service
from   GaudiPython.Bindings import AppMgr
from   GaudiKernel.SystemOfUnits import GeV, second, MeV, keV
# =============================================================================
## test the nodes 
from PartProp.Nodes import *

from Configurables   import LHCbApp
LHCbApp()  

gaudi = AppMgr()
gaudi.initialize()

## simple test-function 
## def test () :
if 1 < 2 : 
    """Simple function for the test
    """
    pps = gaudi.ppSvc()

    c    = pps.find('chi_c1(1P)')

    ## redefine properties 
    line1 = "%s 0 %d 0.0 %g %20.12g %s %d %g " % ( c.name() , c.pid().pid() , c.mass()/GeV ,  c.lifetime()/second , c.evtGen() , c.pythia() , c.maxWidth() / GeV )
    line2 = "%s 0 %d 0.0 %g %20.12g %s %d %g " % ( c.name() , c.pid().pid() , c.mass()/GeV , -c.width   ()/GeV    , c.evtGen() , c.pythia() , c.maxWidth() / GeV )

    pps.Particles = [ line1 ]
    pps.Particles = [ line2 ]
    

# =============================================================================
if '__main__' == __name__ :

    pass 
    ## test  () 

# =============================================================================
# The END 
# =============================================================================
