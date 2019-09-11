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
# Package: L0Muon
################################################################################
gaudi_subdir(L0Muon)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/L0Event
                         L0/L0Base
                         L0/L0Interfaces
                         L0/L0MuonKernel
                         L0/ProcessorKernel
                         DAQ/DAQKernel
                         Muon/MuonDAQ
                         Muon/MuonKernel)

find_package(Boost COMPONENTS system filesystem)
find_package(XercesC)

# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

gaudi_add_module(L0Muon
                 src/component/*.cpp
                 INCLUDE_DIRS Boost Event/DigiEvent L0/L0Interfaces Muon/MuonDAQ
                 LINK_LIBRARIES Boost DetDescLib DAQEventLib DAQKernelLib L0Event L0Base L0MuonKernel ProcessorKernel MuonKernelLib)

