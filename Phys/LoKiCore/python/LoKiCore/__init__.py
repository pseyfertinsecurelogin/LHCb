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
# $Id$
# =============================================================================
## @file  LoKiCore/__init__.py
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2007-05-29
#            Version $Revision$
#  Last modification $Date$
#                 by $Author$
# =============================================================================
""" Helper file to manager LoKiCore package """
_author_ = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
# =============================================================================

try:
    from GaudiKernel import ROOT6WorkAroundEnabled
except ImportError:
    # dummy implementation
    def ROOT6WorkAroundEnabled(id=None):
        return False


if ROOT6WorkAroundEnabled('ROOT-5721'):
    import cppyy
    cppyy.gbl.gROOT.ProcessLine('#define ROOT_5721_WORKAROUND')

# =============================================================================
# The END
# =============================================================================
