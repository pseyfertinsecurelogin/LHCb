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
# Package: GenEvent
################################################################################
gaudi_subdir(GenEvent)

gaudi_depends_on_subdirs(Event/EventBase
                         GaudiObjDesc)

find_package(HepMC)
find_package(ROOT COMPONENTS GenVector)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(GenEvent
                  src/*.cpp
                  PUBLIC_HEADERS GenEvent
                  INCLUDE_DIRS HepMC ROOT Event/EventBase
                  LINK_LIBRARIES HepMC ROOT GaudiObjDescLib)

god_build_dictionary(xml/*.xml
                     EXTEND xml/lcgdict/lcg_selection.h
                     INCLUDE_DIRS HepMC ROOT Event/EventBase
                     LINK_LIBRARIES HepMC ROOT GaudiObjDescLib GenEvent LHCbKernel)
