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
# Package: EventPacker
################################################################################
gaudi_subdir(EventPacker v5r11)

gaudi_depends_on_subdirs(DAQ/MDF
                         Event/DAQEvent
                         Event/HltEvent
                         Event/MCEvent
                         Event/MicroDst
                         Event/RecEvent
                         Event/TrackEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel
                         Kernel/Relations)

find_package(Boost COMPONENTS regex)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_library(EventPackerLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS Event
                  LINK_LIBRARIES MCEvent PhysEvent RecEvent GaudiAlgLib GaudiKernel LHCbKernel RelationsLib )

gaudi_add_module(EventPacker
                 src/component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost MDFLib DAQEventLib HltEvent MCEvent MicroDstLib RecEvent TrackEvent GaudiAlgLib GaudiKernel LHCbKernel RelationsLib EventPackerLib)

gaudi_add_dictionary(EventPacker
                     dict/PackedEventDict.h
                     dict/PackedEventDict.xml
                     LINK_LIBRARIES MCEvent PhysEvent RecEvent GaudiAlgLib GaudiKernel LHCbKernel EventPackerLib)
