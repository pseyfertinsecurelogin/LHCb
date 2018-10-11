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
import argparse
from Configurables import ApplicationMgr
from Configurables import ConfigCDBAccessSvc, PropertyConfigSvc
import cppyy

parser = argparse.ArgumentParser(usage='usage: %(prog)s tck')

parser.add_argument("tck", help="what TCK")

args = parser.parse_args()

tck = int(args.tck, 16)

ApplicationMgr().AppName = ""
ApplicationMgr().OutputLevel = 3

cas_conf = ConfigCDBAccessSvc(File='TCKData/config.cdb', Mode='ReadWrite')
pcs_conf = PropertyConfigSvc(ConfigAccessSvc=cas_conf.getFullName())

import GaudiPython

# Decorate the MD5 object
MD5 = cppyy.gbl.Gaudi.Math.MD5
MD5.__str__ = MD5.str
MD5.__hash__ = lambda x: x.str().__hash__()
MD5.__eq__ = lambda self, other: self.str() == str(other)
MD5.__ne__ = lambda self, other: self.str() != str(other)
digest = MD5.createFromStringRep

ConfigTreeNodeAlias = cppyy.gbl.ConfigTreeNodeAlias
ConfigTreeNodeAlias.alias_type.__str__ = ConfigTreeNodeAlias.alias_type.str
alias = ConfigTreeNodeAlias.alias_type
topLevelAlias = ConfigTreeNodeAlias.createTopLevel
TCK = ConfigTreeNodeAlias.createTCK


def loadAliases(prefix, cas, pcs):
    """Get the existing entries from the access services"""
    r = {}
    for i in cas.configTreeNodeAliases(alias(prefix + '/')):
        tck_id = i.ref().str()
        s = i.alias().str()
        x = pcs.resolveConfigTreeNode(i.ref())
        r[tck_id] = (s, x)
    return r


# create the AppMgr
gaudi = GaudiPython.AppMgr()

# Create the services we need
gaudi.createSvc(cas_conf.getFullName())
cas = gaudi.service(cas_conf.getFullName(), 'IConfigAccessSvc')
gaudi.createSvc(pcs_conf.getFullName())
pcs = gaudi.service(pcs_conf.getFullName(), 'IPropertyConfigSvc')

gaudi.initialize()

# Load the existing aliases
top_aliases = loadAliases('TOPLEVEL', cas, pcs)
tck_aliases = loadAliases('TCK', cas, pcs)

todo = [a for id, (ta, a) in top_aliases.iteritems()
        if id not in tck_aliases][0]

# Create a TCK entry and write it
tck_alias = TCK(todo, tck)
cas.writeConfigTreeNodeAlias(tck_alias)

gaudi.finalize()
gaudi.exit()

print 'PASSED'
