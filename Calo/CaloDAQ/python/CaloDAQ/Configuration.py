#!/usr/bin/env python
# =============================================================================
# @file CaloDAQ/Configuration.py
#  The configuration for CaloDigits  
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-26
# =============================================================================
"""
The configuration for CaloDigits  
"""
# =============================================================================
__author__  = 'Vanya BELYAEV Ivan.Belyaev@nikhef.nl'
__version__ = 'CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.1 $'
# =============================================================================
__all__ = (
    'CaloDigitConf',
    )
# =============================================================================
from Gaudi.Configuration      import *
from LHCbKernel.Configuration import *

from CaloKernel.ConfUtils     import ( addAlgs        ,
                                       printOnDemand  ,
                                       prntCmp        ,
                                       hltContext     , 
                                       setTheProperty )
import logging
_log = logging.getLogger('CaloDAQ')

# =============================================================================
## @class CaloDigitConf
#  Configurable for Calorimeter Digits
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-26
class CaloDigitConf(LHCbConfigurableUser):
    """
    Class/Configurable to define Calo Digits

    """
    ## define the slots
    __slots__ = {
        ##
        "Context"                : "Offline"   # The context to run
        , "MeasureTime"          : False       # Measure the time for sequencers
        , "OutputLevel"          : INFO        # The global output level
        ##
        , 'CreateADCs'           : False       # Create ADCs ? 
        , 'Sequence'             : None        # The sequencer to add the CALO reconstruction algorithms to
        , 'EnableDigitsOnDemand' : False       # Enable Digits-On-Demand
        , 'Detectors'            : ['Ecal','Hcal','Prs','Spd']
        , 'Verbose'              : False
        , 'ReadoutStatusConvert' : False
        ##
        }
    ## documentation lines 
    _propertyDocDct = {
        ##
        "Context"                : """ The context to run """
        , "MeasureTime"          : """ Measure the time for sequencers """
        , "OutputLevel"          : """ The global output level """ 
        ##
        , 'CreateADCs'           : """ Create ADCs ? """
        , 'Sequence'             : """ The sequencer to add the CALO Digit algorithms to """
        , 'EnableDigitsOnDemand' : """ Enable Digits-On-Demand """ 
        , 'Detectors'            : """ List of available calo subdetectors"""
        , 'Verbose'              : """ Verbose printout """
        , 'ReadoutStatusConvert' : """ Add ReadoutStatus -> ProcStatus conversion in the sequence"""
        ##
        }
    
    ## configure processing of Digits
    def digits   ( self ) :
        """
        Configure procession of Digits
        
        """
        from CaloDAQ.CaloDigits import caloDigits
        
        cmp = caloDigits ( self.getProp ('Context'             ) ,
                           self.getProp ('EnableDigitsOnDemand') ,
                           self.getProp ('CreateADCs'          ) ,
                           self.getProp ('Detectors'           ) ,
                           self.getProp ('ReadoutStatusConvert')) 

        return cmp
    
    def printConf(self,verbose=False) :
        if self.getProp ( 'EnableDigitsOnDemand') :
            _log.info    ('CaloDigits onDemand for %s' , self.getProp('Detectors'))
        if self.getProp('Verbose') or verbose:
            _log.info ( self )


    ## Calo Digits Configuration
    def applyConf ( self ) :
        """
        Calorimeter Digits Configuration
        """        
        _log.info ('Apply Calo Digits Configuration')
        self.printConf() 


        cmp = self.digits()    
        setTheProperty ( cmp , 'Context'     , self.getProp ( 'Context'     ) )
        setTheProperty ( cmp , 'OutputLevel' , self.getProp ( 'OutputLevel' ) )
        setTheProperty ( cmp , 'MeasureTime' , self.getProp ( 'MeasureTime' ) )        

      
        if self.isPropertySet('Sequence') :
            seq = self.getProp('Sequence' )
            seq.Members = [ cmp ] + seq.Members  

        if self.getProp ( 'EnableDigitsOnDemand') :
            _log.info    (' creation-on-demand of CaloDigits is enabled for %s' , self.getProp('Detectors'))
            if self.getProp('Verbose') :
                _log.info( printOnDemand() )
            
# =============================================================================
if '__main__' == __name__ :
    print __doc__
    print __author__
    print __version__
    


