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

## @package LHCbKernel
#  Base configurables for the LHCb software
#  @author Chris Jones  (Christopher.Rob.Jones@cern.ch)
#  @date   15/08/2008

__author__ = "Chris Jones <Christopher.Rob.Jones@cern.ch>"

from Gaudi.Configuration import *

# ----------------------------------------------------------------------------------


## @class LHCbConfigurableUser
#  Base configurable for LHCb software
#  @author Chris Jones  (Christopher.Rob.Jones@cern.ch)
#  @date   15/08/2008
class LHCbConfigurableUser(ConfigurableUser):

    ## Default options are empty
    __slots__ = {}

    ## @brief Object 'Constructor'
    def __init__(self, name=Configurable.DefaultName, **kwargs):
        kwargs["name"] = name
        apply(super(LHCbConfigurableUser, self).__init__, (), kwargs)
        self.initialize()

    ## @brief Default initialize method
    #  Derived classes can reimplement this method, to run any object
    #  initialisation tasks they wish
    def initialize(self):
        pass

    ## @brief Set the given property in another configurable object
    #  @param other The other configurable to set the property for
    #  @param name  The property name
    def setOtherProp(self, other, name):
        self.propagateProperty(name, other)

    ## @brief Set the given properties in another configurable object
    #  @param other The other configurable to set the property for
    #  @param names The property names
    def setOtherProps(self, other, names):
        self.propagateProperties(names, other)
