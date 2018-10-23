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
# =============================================================================
## @file
#  Simple script to create proper selection files to buils relations
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#  @date   2006-11-26
# =============================================================================
""" Simple script to create proper selection files to buils relations """
# =============================================================================
__author__ = 'Vanya BELYAEV ibelyaev@physics.syr.edu'

if "__main__" == __name__ :


    import Relations.Rels as Rels 

    # make XML and C++ selction files:
    import phys
    Rels.prepare ( phys.classes ,
                   phys.includes,
                   xmlname = "selPhys.xml",
                   cppname = "selPhys.h" ) 
    import mc
    Rels.prepare ( mc.classes ,
                   mc.includes,
                   xmlname = "selMC.xml",
                   cppname = "selMC.h" ) 
# =============================================================================
# The END 
# =============================================================================
