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
################################################################################
# Package: L0Event
################################################################################
gaudi_subdir(L0Event v19r10)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel)

find_package(Boost)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(L0Event
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost LHCbKernel)

god_build_dictionary(xml/*.xml
                     EXTEND xml/lcgdict/lcg_selection.h
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost LHCbKernel L0Event)

