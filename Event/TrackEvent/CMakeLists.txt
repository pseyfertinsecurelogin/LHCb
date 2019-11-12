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
# Package: TrackEvent
################################################################################
gaudi_subdir(TrackEvent)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Kernel/SOAContainer
                         Kernel/SOAExtensions)

find_package(Boost)
find_package(GSL)
find_package(ROOT)
find_package(Rangev3 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_library(TrackEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS GSL Boost Rangev3 Kernel/SOAContainer Kernel/SOAExtensions
                  LINK_LIBRARIES GSL Boost LHCbKernel )

gaudi_add_unit_test(test_deref tests/src/test_deref.cpp
                    LINK_LIBRARIES GaudiKernel TrackEvent
                    TYPE Boost)
gaudi_add_unit_test(test_track_v2 tests/src/test_track_v2.cpp
                    LINK_LIBRARIES GaudiKernel TrackEvent
                    TYPE Boost)

gaudi_add_dictionary(TrackEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES Boost GSL LHCbKernel TrackEvent INCLUDE_DIRS GSL Boost)
