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
"""
Configuration tools for L0Calo
"""

from Configurables import LHCbConfigurableUser
from Configurables import L0CaloAlg, DataOnDemandSvc, L0CaloCandidatesFromRaw
from Configurables import L0CaloCandidatesFromRawBank, ToolSvc
from Gaudi.Configuration import appendPostConfigAction, log

## @class L0CaloFix2016
# Configurable to configure L0Calo to handle 2016 electron bug
# Definitions:
#   GOOD = the expected correct constants, depend on cell position
#   BAD = the wrong constants loaded in the CALO electronics in 2016,
#         equal to 127 everywhere
# Use cases:
#
# #1 Simulation was done with GOOD constants (eg Sim09b) and the DST contains
#    the CALO RAW banks: in order to represent 2016 data taken with BAD
#    constants, use L0CaloFix2016 with options ApproximateMethod = False
#    and Simulation = True. Set the TCK property to the one you want to
#    simulate (best to use the one that was used for the simulation,
#    ie '0x160F' for most cases. In the DaVinci options, set the DB tags to
#    these used for the simulation.
#
# #2 Simulation was done with GOOD constants (Sim09b) and the DST do not contain
#    the CALO RAW banks: in order to represent 2016 data taken with BAD
#    constants, use L0CaloFix2016 with options ApproximateMethod = True
#    and Simulation = True. Set the TCK property to the one you want to
#    simulate (best to use the one that was used for the simulation,
#    ie '0x160F' for most cases.In the DaVinci options, set the DB tags to
#    these used for the simulation.
#
# #3 Data was recorded in 2016 with BAD constants and you want to re-run
#    L0 with the GOOD constants in order to reject the extra events that
#    were triggered because of the BAD constants, in order to obtain a
#    coherent sample with 2015, 2017 or 2018. Use ApproximateMethod = False
#    and Simulation = False.
#
# NB: Check for messages when running: if there are error messages due
#     to missing CALO banks, do not trust the results. Use the approximate
#     method instead.
# NB: Only L0 is redone, not HLT. If the HLT depends on L0, it has to be
#     ran again.
#
# @author Patrick Robbe <robbe@lal.in2p3.fr>
# @date   2018/03/12

class L0CaloFix2016(LHCbConfigurableUser):

    __slots__ = {
        # Properties
        "ApproximateMethod" : False,
        "Simulation" : True ,
        "TCK" : '0x160F'
        }

    __propertyDocDct = {
        # Properties
        "ApproximateMethod" : """Use approximation on L0Calo to correct energy, otherwise use full method on Calo banks. Useful when Calo banks are not available.""" ,
        "Simulation" : """True for MC, False for data.""",
        "TCK": """The TCK number to emulate"""
        }
    
    def __apply_configuration__(self):
        def fixL0Calo( approximate , simulation , tck ):
            dod = DataOnDemandSvc()
            if not approximate:
                l0calo = L0CaloAlg()
                l0calo.WriteBanks = False
                l0calo.WriteOnTES = True
                l0calo.L0CaloADCTool = "CaloTriggerAdcsFromCaloRaw"
                from Configurables import CaloTriggerAdcsFromCaloRaw 
                l0calo.addTool( CaloTriggerAdcsFromCaloRaw ,
                                "EcalTriggerAdcTool" )
                if simulation:
                    l0calo.EcalTriggerAdcTool.FixFor2016 = True
                else:
                    # for data, take what is in database for the constants
                    l0calo.EcalTriggerAdcTool.FixFor2016 = False
                    
                dod.AlgMap[ 'Trig/L0/FullCalo' ] = l0calo
                dod.AlgMap[ 'Trig/L0/Calo' ] = l0calo
            else:
                l0calo = L0CaloCandidatesFromRaw("L0CaloFromRaw")
                ToolSvc().addTool( L0CaloCandidatesFromRawBank )
                ToolSvc().L0CaloCandidatesFromRawBank.FixFor2016 = True
                if simulation:
                    ToolSvc().L0CaloCandidatesFromRawBank.Simulation = True
                else:
                    # assumes that the correct database tag is given
                    ToolSvc().L0CaloCandidatesFromRawBank.Simulation = False                    
                l0calo.WriteProcData = True
                dod.AlgMap[ 'Trig/L0/FullCalo' ] = l0calo
                dod.AlgMap[ 'Trig/L0/Calo' ] = l0calo
            
            from Configurables import L0DUAlg, L0DUFromRawAlg, GaudiSequencer, L0DUFromRawTool
            l0du = L0DUAlg()
            l0du.WriteBanks = False
            l0du.WriteOnTES = True
            if not approximate:
                dod.AlgMap[ 'Trig/L0/L0DUCaloData' ] = l0calo
                l0du.ProcessorDataLocations = [ 'Trig/L0/L0DUCaloData' , 'Trig/L0/L0DUData' ]
            else:
                dod.AlgMap[ 'Trig/L0/L0DUL0CaloData' ] = l0calo
                l0du.ProcessorDataLocations = [ 'Trig/L0/L0DUL0CaloData' , 'Trig/L0/L0DUData' ]
                
            l0du.TCK = tck
            # CALO
            l0seq = GaudiSequencer("L0Seq")
            l0raw = L0DUFromRawAlg(  WriteProcData = True , WriteOnTES = False )
            l0raw.addTool( L0DUFromRawTool )
            l0raw.L0DUFromRawTool.Emulate = False
            l0seq.Members = [ l0raw , l0du ]
            dod.AlgMap[ 'Trig/L0/L0DUReport' ] = l0seq
 
        log.warning( "Apply 2016 L0Calo fix" )
        if self.getProp( "ApproximateMethod" ):
            if self.getProp( "Simulation" ):
                appendPostConfigAction( lambda tck = self.getProp( "TCK" ): fixL0Calo(True,True,tck) )
            else:
                appendPostConfigAction( lambda tck = self.getProp( "TCK" ): fixL0Calo(True,False,tck) )
        else:
            if self.getProp( "Simulation" ):
                appendPostConfigAction( lambda tck = self.getProp( "TCK" ): fixL0Calo(False,True,tck) )
            else:
                appendPostConfigAction( lambda tck = self.getProp( "TCK" ): fixL0Calo(False,False,tck) )
