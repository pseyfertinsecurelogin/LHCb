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
################################################################################
# Package: Hlt1Event
################################################################################
gaudi_subdir(Hlt1Event v1r0)

gaudi_depends_on_subdirs(Kernel/SOAExtensions Kernel/LHCbKernel Event/DigiEvent)

gaudi_add_unit_test(
  VeloTrack
  tests/src/VeloTrack.cpp
  INCLUDE_DIRS Kernel/LHCbKernel Kernel/SOAContainer Event/DigiEvent
  LINK_LIBRARIES LHCbKernel
  TYPE Boost)

gaudi_install_headers(Event)
