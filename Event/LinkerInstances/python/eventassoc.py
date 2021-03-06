#!/usr/bin/env python
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
# =============================================================================
# =============================================================================
"""
The helper module for manipulations with Linker objects

WARNING: The module is deprecated, please use  LinkerIntances.eventassoc instead

 ---------------------------------------------------------------------------------------------------
 |  OLD semantics  (deprecated)           |          NEW SEMANTICS                                 |
 ---------------------------------------------------------------------------------------------------
 | import eventassoc                      | import LinkerInstances.eventassoc                      |
 | import eventassoc as XXX               | import LinkerInstances.eventassoc as XXX               |
 | from eventassoc import *               | from LinkerInstances.eventassoc import *               |
 | from eventassoc import linkedTo        | from LinkerInstances.eventassoc import linkedTo        |
 | from eventassoc import linkedTo   ax Y | from LinkerInstances.eventassoc import linkedTo   as Y |
 | from eventassoc import linkedFrom      | from LinkerInstances.eventassoc import linkedFrom      |
 | from eventassoc import linkedFrom as X | from LinkerInstances.eventassoc import linkedFrom as X |
 ---------------------------------------------------------------------------------------------------
"""
# =============================================================================
__author__ = "Thomas RUF Thomas.Ruf@cern.ch"
# =============================================================================

print """
eventassoc.py WARNING  Deprecated module to be removed soon
                       Please use LinkerIntances.eventassoc instead

 ---------------------------------------------------------------------------------------------------
 |  OLD semantics  (deprecated)           |          NEW SEMANTICS                                 |
 ---------------------------------------------------------------------------------------------------
 | import eventassoc                      | import LinkerInstances.eventassoc                      |
 | import eventassoc as XXX               | import LinkerInstances.eventassoc as XXX               |
 | from eventassoc import *               | from LinkerInstances.eventassoc import *               |
 | from eventassoc import linkedTo        | from LinkerInstances.eventassoc import linkedTo        |
 | from eventassoc import linkedTo   ax Y | from LinkerInstances.eventassoc import linkedTo   as Y |
 | from eventassoc import linkedFrom      | from LinkerInstances.eventassoc import linkedFrom      |
 | from eventassoc import linkedFrom as X | from LinkerInstances.eventassoc import linkedFrom as X |
 ---------------------------------------------------------------------------------------------------
"""

from LinkerInstances.eventassoc import *

# =============================================================================
if '__main__' == __name__:
    print __doc__
    o = dir()
    for i in o:
        print i

# =============================================================================
# The END
# =============================================================================
