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
# Package: DAQKernel
################################################################################
gaudi_subdir(DAQKernel)

gaudi_depends_on_subdirs(Event/DAQEvent
                         GaudiAlg
                         GaudiUtils
                         Kernel/LHCbKernel)

find_package(ROOT)
find_package(Boost)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

#message(STATUS "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")

gaudi_add_library(DAQKernelLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS DAQKernel
                  LINK_LIBRARIES DAQEventLib GaudiAlgLib GaudiUtilsLib LHCbKernel)


