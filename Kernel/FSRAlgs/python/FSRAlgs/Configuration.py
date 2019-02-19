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
'''
EventAccounting, etc.
'''
## @package LumiAlgs
#  Configuration of simple FSR actions
#  @author Rob lambert, Jaap Panman
#  @date   13/08/2012

__version__ = ""
__author__ = "Jaap Panman, Rob lambert"

from Gaudi.Configuration import *
import GaudiKernel.ProcessJobOptions
from Configurables import LHCbConfigurableUser
