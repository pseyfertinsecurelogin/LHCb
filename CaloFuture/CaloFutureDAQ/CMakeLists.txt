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
# Package: CaloFutureDAQ
################################################################################
gaudi_subdir(CaloFutureDAQ v4r39)

gaudi_depends_on_subdirs(CaloFuture/CaloFutureUtils
                         DAQ/DAQUtils
                         DAQ/DAQKernel
                         Det/CaloDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/L0Event
                         Event/RecEvent
                         GaudiAlg)

find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(CaloFutureDAQ
                 src/*.cpp
                 INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                 LINK_LIBRARIES CaloFutureUtils CaloDetLib DAQEventLib DAQKernelLib L0Event RecEvent GaudiAlgLib)

gaudi_install_python_modules()

