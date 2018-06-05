
#!/usr/bin/env python
# =============================================================================
## @file PartPropSvc.py
#  Demo-file for interactive work with new Particle Property Service
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2008-12-02
# =============================================================================
""" Demo-file for interactive work with new Particle Property Service
"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
# =============================================================================
import PartProp.PartPropAlg
import PartProp.decorators 
import PartProp.Service
from GaudiPython.Bindings import AppMgr
# =============================================================================
## test the nodes 
from PartProp.Nodes import *

from Configurables   import LHCbApp
LHCbApp()  

gaudi = AppMgr()
gaudi.initialize()

## simple test-function 
def test () :
    """Simple function for the test
    """
    pps = gaudi.ppSvc()

    ##  get all known pids
    all  = pps.get( lambda x :  True )


    pids = set()
    for p in  all : pids.add ( p.particleID().pid() )

    ## add "special" values 
    pids.add ( 10022 ) 
    pids.add ( 20022 ) 
    ##
    pids.add ( 30343 )
    pids.add ( 30353 )
    pids.add ( 30363 )
    ##
    pids.add (  1009002   ) 
    pids.add (  1000542   )
    pids.add (  1000522   ) 
    pids.add (  1000039   )
    pids.add (  3100021   )
    pids.add (  480000000 )
    pids.add ( -990000000 ) 

    for p in pids :

        pp = LHCb.ParticleID( p )

        print ( pp.pid             () ,
                pp.abspid          () ,
                pp.isValid         () ,
                pp.isSM            () ,
                pp.isMeson         () ,
                pp.isBaryon        () ,
                pp.isHadron        () ,
                pp.isLepton        () ,
                pp.isNucleus       () ,
                pp.isQuark         () ,
                pp.hasQuarks       () ,
                pp.hasDown         () ,
                pp.hasUp           () ,
                pp.hasStrange      () ,
                pp.hasCharm        () ,
                pp.hasBottom       () ,
                pp.hasTop          () ,
                pp.hasBottomPrime  () ,
                pp.hasTopPrime     () ,
                pp.threeCharge     () ,
                pp.jSpin           () ,
                pp.sSpin           () ,
                pp.lSpin           () ,
                pp.Z               () ,
                pp.A               () ,
                pp.nLambda         () ,
                pp.fundamentalID   () ,
                pp.extraBits       () )
                
                          
if '__main__' == __name__ :

    test  () 

