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
## @file __main__.py
#  @brief Just an entry point for the Browser application.
#
#  If executed (with 'python -m CondDBUI.Browser') it starts the
#  application.
__author__ = "Marco Clemencic <marco.clemencic@cern.ch>"

import sys
from CondDBUI.Browser.Main import main
sys.argv[0] = "CondDBBrowser"  # Set the name of the application
sys.exit(main(sys.argv))
