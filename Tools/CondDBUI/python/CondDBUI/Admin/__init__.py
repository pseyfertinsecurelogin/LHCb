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
"""
Python module for the administration of LHCb Conditions Database.
"""
__author__ = "Marco Clemencic <marco.clemencic@cern.ch>"
__version__ = "$Id: __init__.py,v 1.3 2009-06-30 14:55:08 marcocle Exp $"

from Management import *
# fixme: to be moved into Management
from _internals import (MergeAndTag,
                        MakeDBFromFiles, CompareDBToFiles,
                        DumpToFiles,
                        timegm, timeToValKey)

from ReleaseNotes import *
