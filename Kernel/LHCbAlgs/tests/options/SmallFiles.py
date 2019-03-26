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
from PRConfig import TestFileDB
TestFileDB.test_file_db["2011_smallfiles_EW"].run()

from GaudiConf import IOExtension
IOExtension().inputFiles(
    TestFileDB.test_file_db["2011_smallfiles_EW"].filenames[:5] +
    TestFileDB.test_file_db["2010_justFSR_EW"].filenames +
    TestFileDB.test_file_db["2011_smallfiles_EW"].filenames[5:],
    clear=True)
