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
# Package: MDF
################################################################################
gaudi_subdir(MDF)

gaudi_depends_on_subdirs(Event/DAQEvent
                         GaudiKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_library(MDFLib
                  src/*.cpp
                  PUBLIC_HEADERS MDF
                  INCLUDE_DIRS ROOT
                  LINK_LIBRARIES ROOT GaudiKernel DAQEventLib)

gaudi_add_module(MDF
                 components/*.cpp
                 INCLUDE_DIRS ROOT
                 LINK_LIBRARIES ROOT GaudiKernel DAQEventLib MDFLib)

gaudi_add_test(QMTest QMTEST)
