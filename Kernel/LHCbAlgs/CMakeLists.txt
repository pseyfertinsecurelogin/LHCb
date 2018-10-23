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
# Package: LHCbAlgs
################################################################################
gaudi_subdir(LHCbAlgs v3r9)

gaudi_depends_on_subdirs(Det/DDDB
                         Det/DetDesc
                         Event/DAQEvent
                         DAQ/DAQKernel
                         Event/HltEvent
                         Event/L0Event
                         Event/RecEvent
                         Event/TrackEvent
                         GaudiAlg
                         Kernel/HltInterfaces
                         Kernel/LHCbKernel
                         Kernel/XMLSummaryKernel)

find_package(Boost COMPONENTS filesystem)

find_package(CASTOR) # runtime, only for the tests

find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(LHCbAlgs
                 src/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost DetDescLib DAQEventLib DAQKernelLib HltEvent L0Event RecEvent TrackEvent LHCbKernel HltInterfaces HltEvent)

gaudi_add_module(LHCbAlgsTests tests/src/*.cpp
                 LINK_LIBRARIES LHCbKernel)

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
