###############################################################################
# (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
gaudi_subdir(UTKernel)

gaudi_depends_on_subdirs(Det/UTDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel
                         Si/SiDAQ)

find_package(AIDA)
find_package(Boost)

gaudi_add_library(UTKernelLib
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  INCLUDE_DIRS Boost AIDA Event/DigiEvent Si/SiDAQ
                  LINK_LIBRARIES Boost UTDetLib DAQEventLib RecEvent GaudiAlgLib GaudiKernel LHCbKernel)

gaudi_add_dictionary(UTKernel
                     dict/UTKernelDict.h
                     dict/UTKernelDict.xml
                     INCLUDE_DIRS Boost Event/DigiEvent Si/SiDAQ
                     LINK_LIBRARIES Boost UTDetLib DAQEventLib RecEvent GaudiAlgLib GaudiKernel LHCbKernel UTKernelLib
                     OPTIONS "-DBOOST_DISABLE_ASSERTS")

