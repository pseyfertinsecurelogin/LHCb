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
# @file CaloFutureDAQ/Configuration.py
#  The configuration for CaloFutureDigits
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-26
# =============================================================================
"""
The configuration for CaloFutureDigits
"""
# =============================================================================
__author__ = 'Vanya BELYAEV Ivan.Belyaev@nikhef.nl'
__version__ = '1.1'
# =============================================================================
__all__ = ('CaloFutureDigitConf', )
# =============================================================================
from Gaudi.Configuration import *
from LHCbKernel.Configuration import *

from CaloKernel.ConfUtils import (addAlgs, printOnDemand, prntCmp,
                                  setTheProperty)
import logging
_log = logging.getLogger('CaloFutureDAQ')


# =============================================================================
## @class CaloFutureDigitConf
#  Configurable for CaloFuturerimeter Digits
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-10-26
class CaloFutureDigitConf(LHCbConfigurableUser):
    """
    Class/Configurable to define CaloFuture Digits

    """
    ## define the slots
    __slots__ = {
        ##
        "MeasureTime":
        False  # Measure the time for sequencers
        ,
        "OutputLevel":
        INFO  # The global output level
        ##
        ,
        'CreateADCs':
        False  # Create ADCs ?
        ,
        'Sequence':
        None  # The sequencer to add the CALOFUTURE reconstruction algorithms to
        ,
        'EnableDigitsOnDemand':
        False  # Enable Digits-On-Demand
        ,
        'Detectors': ['Ecal', 'Hcal', 'Prs', 'Spd'],
        'Verbose':
        False,
        'ReadoutStatusConvert':
        False
        ##
    }
    ## documentation lines
    _propertyDocDct = {
        ##
        "MeasureTime":
        """ Measure the time for sequencers """,
        "OutputLevel":
        """ The global output level """
        ##
        ,
        'CreateADCs':
        """ Create ADCs ? """,
        'Sequence':
        """ The sequencer to add the CALOFUTURE Digit algorithms to """,
        'EnableDigitsOnDemand':
        """ Enable Digits-On-Demand """,
        'Detectors':
        """ List of available calo subdetectors""",
        'Verbose':
        """ Verbose printout """,
        'ReadoutStatusConvert':
        """ Add ReadoutStatus -> ProcStatus conversion in the sequence"""
        ##
    }

    ## configure processing of Digits
    def digits(self):
        """
        Configure procession of Digits

        """
        from CaloFutureDAQ.CaloFutureDigits import caloDigits

        cmp = caloDigits(
            self.getProp('CreateADCs'), self.getProp('Detectors'),
            self.getProp('ReadoutStatusConvert'))

        return cmp

    def printConf(self, verbose=False):
        if self.getProp('EnableDigitsOnDemand'):
            _log.info('CaloFutureDigits onDemand for %s',
                      self.getProp('Detectors'))
        if self.getProp('Verbose') or verbose:
            _log.info(self)

    ## CaloFuture Digits Configuration
    def applyConf(self):
        """
        CaloFuturerimeter Digits Configuration
        """
        _log.info('Apply CaloFuture Digits Configuration')
        self.printConf()

        cmp = self.digits()
        setTheProperty(cmp, 'MeasureTime', self.getProp('MeasureTime'))
        if self.isPropertySet("OutputLevel"):
            setTheProperty(cmp, 'OutputLevel', self.getProp('OutputLevel'))

        if self.isPropertySet('Sequence'):
            seq = self.getProp('Sequence')
            seq.Members = [cmp] + seq.Members

        if self.getProp('EnableDigitsOnDemand'):
            _log.info(
                ' creation-on-demand of CaloFutureDigits is enabled for %s',
                self.getProp('Detectors'))
            if self.getProp('Verbose'):
                _log.info(printOnDemand())


# =============================================================================
if '__main__' == __name__:
    print __doc__
    print __author__
    print __version__
