#!/usr/bin/env python
###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
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
__author__ = 'Vanya BELYAEV Ivan.Belyaev@nikhef.nl'
__version__ = '1.2'
# =============================================================================
from Gaudi.Configuration import *

import logging
_log = logging.getLogger('CaloFutureDAQ')


# =============================================================================
## the digits
def caloDigits(createADCs=False,
               detectors=['Ecal', 'Hcal'],
               ReadoutStatusConvert=False):
    """
    Decoding of CaloFuture-Digits
    """
    _cntx = 'Offline'

    from Configurables import (CaloFutureRawToDigits,
                               RawBankReadoutStatusConverter, GaudiSequencer)

    conflist = []
    alglist = []

    if 'Ecal' in detectors:
        _log.debug('caloDigits : Ecal is added to the detector list')
        ecalSeq = GaudiSequencer('FutureEcalDigitsSeq', Context=_cntx)
        ecal = CaloFutureRawToDigits("FutureEcalZSup")
        from cppyy.gbl import DeCalorimeterLocation
        ecal.DetectorLocation = DeCalorimeterLocation.Ecal
        ecal.ZSupMethod = "2D"
        ecal.ZSupThreshold = 20
        ecal.ZSupNeighbour = -5

        conflist.append(ecal)
        if ReadoutStatusConvert:
            ecalCnv = RawBankReadoutStatusConverter("EcalProcStatus")
            ecalCnv.System = 'Ecal'
            ecalCnv.BankTypes = ['EcalPacked']
            ecalSeq.Members = [ecal, ecalCnv]
            alglist.append(ecalSeq)
        else:
            alglist.append(ecal)

    if 'Hcal' in detectors:
        _log.debug('caloDigits : Hcal is added to the detector list')
        hcalSeq = GaudiSequencer('FutureHcalDigitsSeq', Context=_cntx)
        hcal = CaloFutureRawToDigits("FutureHcalZSup")
        from cppyy.gbl import DeCalorimeterLocation
        hcal.DetectorLocation = DeCalorimeterLocation.Hcal
        hcal.ZSupMethod = "1D"
        hcal.ZSupThreshold = 4

        conflist.append(hcal)
        if ReadoutStatusConvert:
            hcalCnv = RawBankReadoutStatusConverter("HcalProcStatus")
            hcalCnv.System = 'Hcal'
            hcalCnv.BankTypes = ['HcalPacked']
            hcalSeq.Members = [hcal, hcalCnv]
            alglist.append(hcalSeq)
        else:
            alglist.append(hcal)

    if createADCs:
        for a in conflist:
            t = a.OutputType.upper()
            if t in ['DIGIT', 'DIGITS', 'CALOFUTUREDIGIT', 'CALOFUTUREDIGITS']:
                t.OutputType = 'Both'
                _log.warning('Change OutputType for %s from %s to %' %
                             (t.getFullName(), t, t.OutputType))
            elif t in ['ADC', 'ADCS', 'CALOFUTUREADC', 'CALOFUTUREADCS']:
                pass
            elif t is 'BOTH':
                pass
            else:
                t.OutputType = 'Adc'
                _log.warning('Change OutputType for %s from %s to %' %
                             (t.getFullName(), t, t.OutputType))

    ## combine them into sequence
    alg = GaudiSequencer(
        'CaloFutureDigits',
        Context=_cntx,
        IgnoreFilterPassed=True,
        Members=alglist)
    return alg


# =============================================================================
if '__main__' == __name__:
    print __doc__
    print __author__
    print __version__

# =============================================================================
# The END
# =============================================================================
