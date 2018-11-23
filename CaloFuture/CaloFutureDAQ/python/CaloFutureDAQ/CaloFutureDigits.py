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
# =============================================================================
# @file CaloFutureDAQ/CaloFutureDigits.py
#  The configuration for CaloFutureDigits  
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-26
# =============================================================================
"""
The configuration for CaloFutureDigits  
"""
# =============================================================================
__author__  = 'Vanya BELYAEV Ivan.Belyaev@nikhef.nl'
__version__ = 'CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.2 $'
# =============================================================================
from Gaudi.Configuration import *

from CaloKernel.ConfUtils import getAlgo


import logging
_log = logging.getLogger('CaloFutureDAQ')

# =============================================================================
## the digits 
def caloDigits ( context        ,
                 enableOnDemand ,
                 createADCs=False,
                 detectors=['Ecal','Hcal','Prs','Spd'],
                 ReadoutStatusConvert=False) :

    """
    Decoding of CaloFuture-Digits 
    """
    _cntx = 'Offline'
    if context != _cntx :
        _log.warning('CaloFutureDigit: Context is redefined to be Offline')
        
    from Configurables import ( CaloFutureRawToDigits   ,
                                RawBankReadoutStatusConverter,
                                GaudiSequencer    ) 
    

 

    conflist=[]
    alglist=[]
        
    if 'Ecal' in detectors :
        _log.debug('caloDigits : Ecal is added to the detector list')
        ecalSeq = GaudiSequencer ('FutureEcalDigitsSeq',Context = _cntx)
        ecal = getAlgo ( CaloFutureRawToDigits             , 
                         "FutureEcalZSup"              ,
                         _cntx                   ,
                         "Raw/Ecal/Digits"       ,
                         enableOnDemand          )
        ecal.DetectorName = "Ecal"
        ecal.ZSupMethod = "2D"
        ecal.ZSupThreshold = 20
        ecal.ZSupNeighbour = -5

        conflist.append(ecal)
        if ReadoutStatusConvert :
            ecalCnv = getAlgo ( RawBankReadoutStatusConverter, "EcalProcStatus",_cntx)
            ecalCnv.System='Ecal'
            ecalCnv.BankTypes=['EcalPacked']
            ecalSeq.Members = [ecal,ecalCnv]
            alglist.append(ecalSeq)
        else :
            alglist.append(ecal)


    if 'Hcal' in detectors :
        _log.debug('caloDigits : Hcal is added to the detector list')
        hcalSeq = GaudiSequencer ('FutureHcalDigitsSeq',Context = _cntx)
        hcal = getAlgo ( CaloFutureRawToDigits         , 
                         "FutureHcalZSup"              ,
                         _cntx                   ,
                         "Raw/Hcal/Digits"       ,
                         enableOnDemand          )
        hcal.DetectorName = "Hcal"
        hcal.ZSupMethod = "1D"
        hcal.ZSupThreshold = 4

        conflist.append(hcal)
        if ReadoutStatusConvert :
            hcalCnv = getAlgo ( RawBankReadoutStatusConverter, "HcalProcStatus",_cntx)
            hcalCnv.System='Hcal'
            hcalCnv.BankTypes=['HcalPacked']
            hcalSeq.Members = [hcal,hcalCnv]
            alglist.append(hcalSeq)
        else :
            alglist.append(hcal)



    if createADCs :
        for a in conflist :
            t = a.OutputType.upper()
            if   t in [ 'DIGIT', 'DIGITS' , 'CALOFUTUREDIGIT' , 'CALOFUTUREDIGITS' ] :
                t.OutputType = 'Both'
                _log.warning( 'Change OutputType for %s from %s to %'
                              % ( t.getFullName() , t , t.OutputType ) )
            elif t in [ 'ADC'  , 'ADCS'   , 'CALOFUTUREADC'   , 'CALOFUTUREADCS'   ] :
                pass
            elif t is 'BOTH' :
                pass
            else :
                t.OutputType = 'Adc'
                _log.warning( 'Change OutputType for %s from %s to %'
                              % ( t.getFullName() , t , t.OutputType ) )

               
            
    ## combine them into sequence  
    alg = GaudiSequencer ('CaloFutureDigits',Context = context, IgnoreFilterPassed = True, Members=alglist)
    return alg 
    
    
# =============================================================================
if '__main__' == __name__ :
    print __doc__
    print __author__
    print __version__
    

# =============================================================================
# The END 
# =============================================================================
