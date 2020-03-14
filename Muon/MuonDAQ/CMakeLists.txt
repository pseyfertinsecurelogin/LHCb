###############################################################################
# (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
gaudi_subdir(MuonDAQ)

gaudi_depends_on_subdirs(DAQ/DAQUtils
                         Det/MuonDet
                         Kernel/LHCbKernel
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         GaudiAlg
                         DAQ/DAQKernel
                         Muon/MuonKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_library(MuonDAQLib
                  src/*.cpp
                  PUBLIC_HEADERS Event MuonDAQ
                  LINK_LIBRARIES MuonDetLib LHCbKernel)

gaudi_add_module(MuonDAQ
                 src/components/*.cpp
                 INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                 LINK_LIBRARIES MuonDetLib DAQEventLib DAQKernelLib RecEvent GaudiAlgLib MuonKernelLib MuonDAQLib)

gaudi_add_dictionary(MuonDAQ
                     dict/MuonDAQDict.h
                     dict/MuonDAQDict.xml
                     INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                     LINK_LIBRARIES MuonDetLib DAQEventLib RecEvent GaudiAlgLib MuonKernelLib
                     OPTIONS "-DBOOST_DISABLE_ASSERTS")

gaudi_add_test(QMTest QMTEST)
