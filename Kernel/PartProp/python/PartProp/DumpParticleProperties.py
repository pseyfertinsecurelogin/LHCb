#!/usr/bin/env python
# =============================================================================
# $Id:$ 
# =============================================================================
## @file DumpParticleProperties.py
#  Demo-file for interactive work with new Particle Property Service
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2010-10-22
# =============================================================================
"""Trivial script to dump the table of Particle Properties 
"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
# =============================================================================
import sys

def dump ( args = [] ) :
    
    from argparse import ArgumentParser 
    parser = ArgumentParser (
        prog        = 'dump_particle_properties' , 
        description = """
        Dump the table of particle properties
        Optionally specify the DDDB and CONDDB ags, datatype and Simulation flags
        """,
        )
    #
    parser.add_argument ( 
        "-d" , "--DDDB"         ,
        type    = str           ,
        dest    = 'DDDBtag'     , 
        help    = "DDDB tag to be used [default: %(default)s]" ,
        default = ''            )
    parser.add_argument ( 
        "-c" , "--CONDDB"       ,
        type    = str           ,
        dest    = 'CONDDBtag'   , 
        help    = "CONDDB tag to be used [default: %(default)s]" ,
        default = ''            )
    parser.add_argument ( 
        "-t" , "--DATATYPE"     ,
        type    = str           ,
        dest    = 'DataType'    , 
        help    = "DataType to be used [default: %(default)s]" ,
        default = ''            )
    parser.add_argument ( 
        "-s" , "--Simulation"   ,
        action  = "store_true"  ,
        dest    = 'Simulation'  , 
        help    = "Simulation ? [default: %(default)s]" ,
        default = False         )

    ## eliminate artifacts and parse command-line arguments 
    if not args : args = [ a for a in sys.argv[1:] if '--' != a ] 
    arguments = parser.parse_args( args )  

    
    import PartProp.PartPropAlg
    import PartProp.Service
    
    from   Configurables   import MessageSvc 
    MessageSvc ( OutputLevel = 6 )
    
    ## configure LHCbApp 
    from   Configurables   import LHCbApp
    app = LHCbApp ( OutputLevel = 6 )
    if arguments.DDDBtag   : app.DDDBtag   = arguments.DDDBtag 
    if arguments.CONDDBtag : app.CondDBtag = arguments.CONDDBtag 
    if arguments.DataType  : app.DataType  = arguments.DataType
    
    app.Simulation = arguments.Simulation
    
    from   GaudiPython.Bindings import AppMgr
    gaudi = AppMgr()
    
    gaudi.initialize()
    
    pps   = gaudi.ppSvc()
    
    print pps.all()

    print app
    
# =============================================================================
if '__main__' == __name__ :

    dump( [ a for a in sys.argv[1:] if '--' != a ] ) 

# =============================================================================
# The END 
# =============================================================================
