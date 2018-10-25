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
# Package: FTDAQ
################################################################################
gaudi_subdir(FTDAQ v1r8)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/FTEvent
                         DAQ/DAQKernel
                         Det/DetDesc
                         Det/FTDet
                         GaudiAlg)

find_package(Boost)

gaudi_add_module(FTDAQ
                 src/*.cpp
                 INCLUDE_DIRS Boost Event/FTEvent
                 LINK_LIBRARIES DAQEventLib DAQKernelLib GaudiAlgLib DetDescLib FTDAQLib)

gaudi_add_library(FTDAQLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS FTDAQ
                  INCLUDE_DIRS Boost Event/RawEvent
                  LINK_LIBRARIES DAQEventLib)
