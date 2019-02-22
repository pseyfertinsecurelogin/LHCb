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
import importUtils


#================================================================================
class genPackage(importUtils.importUtils):
    #--------------------------------------------------------------------------------
    def __init__(self, godPackage):
        importUtils.importUtils.__init__(self)
        self.godPackage = godPackage
        self.dict = {}
        self.include = []
        self.stdIncludes = []
        self.forwardDeclLHCb = []
        self.forwardDeclGlob = {}
        self.forwardIncl = []
        self.excludes = []
        self.doit()
#--------------------------------------------------------------------------------

    def parsePackImport(self, dict):
        self.parseImport(dict, self.include, self.stdIncludes,
                         self.forwardDeclLHCb, self.forwardDeclGlob,
                         self.forwardIncl)


#--------------------------------------------------------------------------------

    def doit(self):
        self.dict['packagename'] = self.godPackage['attrs']['name']
        self.dict['packagenamespace'] = self.godPackage['attrs']['namespace']
        self.parsePackImport(self.godPackage)
