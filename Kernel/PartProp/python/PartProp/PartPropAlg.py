#!/usr/bin/env gaudirun.py
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
## @file
#  The configurationi file to run QMTest for the package Kernel/PartProp
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date   2008-12-02
# =============================================================================
""" The configuration file to run QMTest for the package Kernel/PartProp
"""
# =============================================================================
__author__  =  "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
# =============================================================================

from Gaudi.Configuration import *
from Configurables import ApplicationMgr 
from Configurables import LHCb__ParticlePropertySvc  as PPS 
from Configurables import LHCb__Example__PartPropAlg as PPA 

## needed for the proper access to Particle table  from DDDB  
from Configurables import DDDBConf 
DDDBConf       ()  # use the default Configuration of DDDB

ApplicationMgr ( TopAlg = [ PPA() ] ,
                 EvtSel = "NONE"    ,
                 EvtMax = 1         )
# =============================================================================
# The END 
# =============================================================================
