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
# Package: PhysEvent
################################################################################
gaudi_subdir(PhysEvent v11r28)

gaudi_depends_on_subdirs(Event/RecEvent
                         GaudiKernel
                         GaudiObjDesc
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(PhysEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  LINK_LIBRARIES RecEvent GaudiKernel LHCbKernel LHCbMathLib)

god_build_dictionary(xml/*.xml
                     EXTEND dict/lcgDict.h
                     LINK_LIBRARIES RecEvent GaudiKernel LHCbKernel LHCbMathLib PhysEvent)

