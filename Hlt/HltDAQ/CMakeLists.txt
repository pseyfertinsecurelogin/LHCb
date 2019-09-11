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
# Package: HltDAQ
################################################################################
gaudi_subdir(HltDAQ)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/DAQEvent
                         Event/HltEvent
                         Event/LumiEvent
                         Event/EventPacker
                         GaudiAlg
                         GaudiKernel
                         GaudiObjDesc
                         Hlt/HltServices
                         Kernel/LHCbMath
                         Kernel/HltInterfaces
                         Phys/LoKiHlt
                         DAQ/DAQKernel)

find_package(AIDA)
find_package(Boost COMPONENTS iostreams filesystem)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

include(GaudiObjDesc)

god_build_headers(xml/*.xml
                  DESTINATION HltDAQ)

gaudi_add_library(HltDAQLib
                 src/lib/*.cpp
                 PUBLIC_HEADERS HltDAQ
                 INCLUDE_DIRS Boost AIDA
                 LINK_LIBRARIES Boost DetDescLib DAQEventLib DAQKernelLib HltEvent GaudiAlgLib GaudiKernel HltInterfaces LoKiHltLib)

gaudi_add_module(HltDAQ
                 src/component/*.cpp
                 INCLUDE_DIRS Boost AIDA HltDAQ
                 LINK_LIBRARIES HltDAQLib Boost DetDescLib DAQEventLib DAQKernelLib HltEvent GaudiAlgLib GaudiKernel HltInterfaces LoKiHltLib)

gaudi_add_unit_test(utestTrackingCoder
                    src/utest/utestTrackingCoder.cpp
                    LINK_LIBRARIES GaudiKernel HltEvent HltDAQLib
                    TYPE Boost)

gaudi_add_test(QMTest QMTEST)
