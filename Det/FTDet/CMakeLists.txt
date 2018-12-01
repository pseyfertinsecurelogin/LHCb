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
# Package: FTDet
################################################################################
gaudi_subdir(FTDet v2r13)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         Event/FTEvent
                         Event/MCEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost)
find_package(ROOT)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS}
                           ${ROOT_INCLUDE_DIRS}
                           ${XERCESC_INCLUDE_DIRS})

gaudi_add_library(FTDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS FTDet
                  INCLUDE_DIRS Boost Event/FTEvent
                  LINK_LIBRARIES Boost DetDescCnvLib MCEvent GaudiAlgLib GaudiKernel LHCbKernel LHCbMathLib)

gaudi_add_module(FTDet
                 src/component/*.cpp
                 INCLUDE_DIRS Boost Event/FTEvent
                 LINK_LIBRARIES Boost DetDescCnvLib MCEvent GaudiAlgLib GaudiKernel LHCbKernel LHCbMathLib FTDetLib)

gaudi_add_dictionary(FTDet
                     dict/FTDetDict.h
                     dict/FTDetDict.xml
                     INCLUDE_DIRS Boost Event/FTEvent
                     LINK_LIBRARIES Boost DetDescCnvLib MCEvent GaudiAlgLib GaudiKernel LHCbKernel LHCbMathLib FTDetLib
                     OPTIONS "-U__MINGW32__")

