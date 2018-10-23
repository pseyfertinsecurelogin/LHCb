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
# $Id: MaterialBudget.py,v 1.1 2009-11-15 16:46:00 ibelyaev Exp $
# =============================================================================
## @file DetDescChecks/MaterialBidget.py
#  Helper file to illustrate the usage of Material-Budget algorithm
#  @see DetDesc::CheckOverlap 
#  @author Vanya BELYAEV Icvan.Belyaev@nikhef.nl
# =============================================================================
"""
Helper file to illustrate the usage of Material-Budget algorithm
"""
# =============================================================================
__author__  = 'Vanya BELYAEV Icvan.Belyaev@nikhef.nl'
__version__ = 'CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.1 $'
# =============================================================================

from Gaudi.Configuration import *

from Configurables  import DDDBConf
DDDBConf ( DataType = '2015' )

from Configurables import DetDesc__MaterialBudget

budget = DetDesc__MaterialBudget (
    'Budget' ,
    #
    zPlane   = 970.0 ,
    Shots    = 100   ,
    #
    Grid     = False ,
    Rapidity = False , 
    #
    )

from Configurables import ApplicationMgr

ApplicationMgr  (
    TopAlg = [ budget ] ,
    EvtSel = 'NONE'     , 
    EvtMax = 1         
    )

# =============================================================================
if "__main__" == __name__  :

    print __doc__
    print __author__
    print __version__
    
    from GaudiPython.Bindings import AppMgr
    
    gaudi = AppMgr()
    
    gaudi.run(1) 
    
# =============================================================================
# The END 
# =============================================================================

