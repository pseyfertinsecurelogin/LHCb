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
## @file Main.py
#  @brief Main module
#
#  This module contains the "main" function, which initializes the application
#  and starts the event loop.
__author__ = "Marco Clemencic <marco.clemencic@cern.ch>"

import logging
import sys


## Initialize and start the application.
#  If an argument is passed on the command line, it is used to open the corresponding database
#  retrieved from the options file.
#  @see getStandardConnectionStrings()
def main(argv=None):
    from PyQt5.Qt import QApplication
    from MainWindow import MainWindow

    try:
        from __version__ import __versionNumber__, __versionDate__
    except ImportError:
        __versionNumber__ = __versionDate__ = 'unknown'

    if argv is None:
        argv = sys.argv

    # Initializes the Qt application
    app = QApplication(argv)
    app.setApplicationName(app.objectName())
    app.setApplicationVersion("%s (%s)" % (__versionNumber__, __versionDate__))
    app.setOrganizationName("LHCb")
    app.setOrganizationDomain("lhcb.cern.ch")
    # @FIXME: I need a flexible way of defining the default style (command line, settings, ...)
    # app.setStyle("plastique")

    # Get the list of command line arguments after the filter applied by QApplication
    # and convert them to strings so that we can pass the to optparse
    argv = map(str, app.arguments())

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [Qt-options] [options] [database]")
    parser.add_option(
        "--rw",
        action="store_true",
        help=
        "Open the database specified in the command line in read/write mode")
    parser.add_option(
        "--verbose", action="store_true", help="Increase verbosity")
    parser.set_defaults(rw=False)

    opts, args = parser.parse_args(argv[1:])

    logging.basicConfig(level=opts.verbose and logging.INFO or logging.WARNING)

    if len(args) > 1:
        parser.error(
            "Only one database can be specified on the command line (see --help)."
        )

    mw = MainWindow()
    from DDDB.Configuration import GIT_CONDDBS
    mw.setDefaultDatabases(GIT_CONDDBS)

    # Use the first (and only) argument as name of the database to open
    if args:
        db = args[0]
        readOnly = not opts.rw
        if db in mw.defaultDatabases:
            mw.openStandardDatabase(db, readOnly=readOnly)
        else:
            mw.openDatabase(db, readOnly=readOnly)

    mw.show()

    # Execute the event loop
    return app.exec_()
