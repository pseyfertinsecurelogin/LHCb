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
from GaudiTesting.BaseTest import LineSkipper
from GaudiConf.QMTest.LHCbExclusions import preprocessor as LHCbPreprocessor

preprocessor = LHCbPreprocessor + \
  LineSkipper(["FileRecordCnvSvc     INFO File sharing enabled. Do not retire files"]) + \
  LineSkipper(["DEBUG Releasing service 'UpdateManagerSvc'"]) + \
  LineSkipper(["LHCBCOND_"]) + \
  LineSkipper(["DDDB_"])
