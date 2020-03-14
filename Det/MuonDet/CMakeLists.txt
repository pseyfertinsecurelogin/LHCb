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
gaudi_subdir(MuonDet)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         GaudiAlg
                         Kernel/LHCbKernel
                         Muon/MuonKernel)

find_package(ROOT)
find_package(Boost)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})

gaudi_add_library(MuonDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS MuonDet
                  LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib)

gaudi_add_module(MuonDet
                 src/component/*.cpp
                 LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib MuonDetLib)

gaudi_add_dictionary(MuonDet
                     dict/MuonDetDict.h
                     dict/MuonDetDict.xml
                     LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib MuonDetLib)

