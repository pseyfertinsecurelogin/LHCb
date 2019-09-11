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
# Package: STDAQ
################################################################################
gaudi_subdir(STDAQ)

gaudi_depends_on_subdirs(DAQ/DAQUtils
                         Det/DetDesc
                         Det/STDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel
                         ST/STKernel
                         ST/STTELL1Event
                         Si/SiDAQ)


find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(STDAQ
                 src/*.cpp
                 INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent Si/SiDAQ
                 LINK_LIBRARIES DetDescLib STDetLib DAQEventLib RecEvent GaudiAlgLib GaudiKernel LHCbKernel STKernelLib STTELL1Event)

