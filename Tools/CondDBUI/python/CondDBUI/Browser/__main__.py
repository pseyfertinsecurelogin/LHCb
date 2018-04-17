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
