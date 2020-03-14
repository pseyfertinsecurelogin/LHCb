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
gaudi_subdir(OTDet)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost)
find_package(ROOT)
find_package(XercesC)
find_package(VDT)
find_package(VectorClass)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS} ${VectorClass_INCLUDE_DIR})

gaudi_add_library(OTDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS OTDet
                  INCLUDE_DIRS Boost VDT ${VectorClass_INCLUDE_DIR}
                  LINK_LIBRARIES Boost DetDescCnvLib LHCbKernel LHCbMathLib)

gaudi_add_module(OTDet
                 src/component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost DetDescCnvLib LHCbKernel LHCbMathLib OTDetLib)

gaudi_add_dictionary(OTDet
                     dict/OTDetDict.h
                     dict/OTDetDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost DetDescCnvLib LHCbKernel LHCbMathLib OTDetLib)

