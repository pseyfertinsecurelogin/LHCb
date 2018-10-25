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

# ============ Auto-generated testing python file ======
from Gaudi.Configuration import *
import Configurables
from GaudiKernel.ConfigurableDb import cfgDb, loadConfigurableDb
import sys, traceback

##### load all configurables of this package #####
loadConfigurableDb()
for name,conf in cfgDb.iteritems():
  if conf['package']=='RawEventCompat':
    if hasattr(Configurables,name):
      try:
        aConf=getattr(Configurables,name)
        aConf()
      except Exception, e:
        print >> sys.stderr, 'ERROR, cannot import/instantiate configurable', name, '\n-------\n', e.__class__, '\n-------'
        traceback.print_exc()


# =========== Auto-generated, import all python modules ====
import glob, os, sys, traceback
modules=glob.glob('../../python/RawEventCompat/*.py')
for mod in modules:
  try:
    amod=__import__('RawEventCompat.'+mod.split(os.sep)[-1][:-3])
  except Exception, e:
    print >> sys.stderr, 'ERROR, cannot import module', mod, '\n-------\n', e.__class__, '\n-------',
    traceback.print_exc()

