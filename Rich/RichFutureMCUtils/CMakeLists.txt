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
# Package: RichFutureMCUtils
################################################################################
gaudi_subdir(RichFutureMCUtils)

gaudi_depends_on_subdirs(GaudiUtils
                         Kernel/LHCbKernel
                         Event/MCEvent
                         Event/RecEvent
                         Event/TrackEvent
                         Event/LinkerEvent
                         Kernel/Relations
                         Rich/RichFutureKernel
                         Rich/RichFutureUtils)

find_package(Vc)
include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_library(RichFutureMCUtilsLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS RichFutureMCUtils
                  INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                  LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )

gaudi_add_module(RichFutureMCUtils
                 src/component/*.cpp
                 INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                 LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel RichFutureMCUtilsLib GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )

