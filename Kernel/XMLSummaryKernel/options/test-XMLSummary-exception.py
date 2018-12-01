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
# Test, will read and write a Dst file
# Will fill some counters, and output the summary.xml
# This should throw an exception after 3 events
# These options are mostly copied from $GAUDIEXAMPLESROOT/python/CounterEx.py
from Gaudi.Configuration import *

importOptions("$XMLSUMMARYKERNELROOT/options/test-XMLSummary.py")


#---------------------------------------------------------------------------
# Setup read and write of a POOL dst file
#---------------------------------------------------------------------------
import GaudiPython

from   GaudiPython.GaudiAlgs   import GaudiAlgo

SUCCESS = GaudiPython.SUCCESS 

# =============================================================================
# Simple algorithm which manipulates with counters 
# =============================================================================
class Counter(GaudiAlgo) :
    """ Simple algorithm which manipulates with counters """
    def __init__ ( self , name = 'Counter' ) :
        """ Constructor """
        GaudiAlgo.__init__( self , name )
        
    def execute( self ) :
        """ The major method 'execute', it is invoked for each event """

        executed = self.counter('executed')
        executed += 1. 

        if executed.flag() > 3:
            raise ValueError, 'Deliberately thrown error on the fourth event'
            return GaudiPython.FAILURE
        return SUCCESS


# =============================================================================
# The actual job excution 
# =============================================================================
gaudi = GaudiPython.AppMgr()
gaudi.config()
alg = Counter()
gaudi.setAlgorithms( [alg] )
gaudi.run(5)
  
# =============================================================================
# The END 
# =============================================================================
