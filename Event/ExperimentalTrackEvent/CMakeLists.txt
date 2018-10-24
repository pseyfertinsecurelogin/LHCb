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
# Package: ExperimentalTrackEvent
################################################################################
gaudi_subdir(ExperimentalTrackEvent v1r0)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Kernel/SOAContainer
                         Event/TrackEvent)

gaudi_add_library(ExperimentalTrackEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Boost ROOT SOAContainer
                  LINK_LIBRARIES LHCbKernel Boost TrackEvent)
