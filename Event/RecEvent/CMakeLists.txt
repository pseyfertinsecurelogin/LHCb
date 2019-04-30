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
# Package: RecEvent
################################################################################
gaudi_subdir(RecEvent v2r57)

gaudi_depends_on_subdirs(Event/DigiEvent
                         Event/EventBase
                         Event/TrackEvent
                         GaudiObjDesc
                         Kernel/LHCbMath
                         Kernel/PartProp
                         Kernel/SOAContainer)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(RecEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Event/DigiEvent Event/EventBase Kernel/SOAContainer
                  LINK_LIBRARIES TrackEvent LHCbMathLib PartPropLib)

god_build_dictionary(xml/*.xml
                     EXTEND dict/lcgDict.h
                     INCLUDE_DIRS Event/DigiEvent Event/EventBase Kernel/SOAContainer
                     LINK_LIBRARIES TrackEvent LHCbMathLib PartPropLib RecEvent)

