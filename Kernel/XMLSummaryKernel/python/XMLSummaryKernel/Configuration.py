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
"""
Configuration of the XMLSummarySvc
"""
__version__ = "$Id: Configuration.py,v 1.4 2009-10-07 13:49:17 rlambert Exp $"
__author__  = "Rob Lambert"

from Gaudi.Configuration import *

class XMLSummary(ConfigurableUser):
    __slots__ = {
        "XMLSummary"   : None
        }
    
    _propertyDocDct = { 
       'XMLSummary'   : """ Add an XML summary file, default None """
       }
    
    def __apply_configuration__(self):
        if hasattr( self, "XMLSummary" ):
            if self.getProp("XMLSummary") is None: return
            from Configurables import ( XMLSummarySvc )
            xmlSum=XMLSummarySvc("CounterSummarySvc")
            xmlSum.xmlfile=self.getProp("XMLSummary")
            ApplicationMgr().ExtSvc+=[ xmlSum ]
            if "Gaudi::IODataManager/IODataManager" not in ApplicationMgr().ExtSvc:
                ApplicationMgr().ExtSvc+=[ "Gaudi::IODataManager/IODataManager" ]
            for aSvc in ApplicationMgr().ExtSvc:
                if "/FileCatalog" in aSvc:
                    return
            #only add the filecatalog if it doesn't exist
            ApplicationMgr().ExtSvc+=[ "Gaudi::MultiFileCatalog/FileCatalog" ]
            
