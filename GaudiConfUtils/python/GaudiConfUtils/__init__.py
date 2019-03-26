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
"""Module with all kinds of tools to manipulate Configurables and Configurable modules.

@author Juan Palacios palacios@physik.uzh.ch

"""

from Utils import isIterable
from Utils import isConfigurable
from Utils import configurableExists
from Utils import filterConfigurables
from Utils import getConfigurablesFromModule

__all__ = ('isIterable', 'isConfigurable', 'configurableExists',
           'filterConfigurables', 'getConfigurablesFromModule')

__author__ = 'Juan Palacios palacios@physik.uzh.ch'
