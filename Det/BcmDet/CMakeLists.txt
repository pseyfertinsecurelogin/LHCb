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
# Package: BcmDet
################################################################################
gaudi_subdir(BcmDet)

gaudi_depends_on_subdirs(Det/DetDesc
                         Det/DetDescCnv
                         GaudiKernel)

find_package(ROOT)
find_package(Boost)
find_package(XercesC)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS}
                           ${XERCESC_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS})

gaudi_add_library(BcmDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS BcmDet
                  LINK_LIBRARIES DetDescLib DetDescCnvLib GaudiKernel)

gaudi_add_module(BcmDet
                 src/component/*.cpp
                 LINK_LIBRARIES DetDescLib DetDescCnvLib GaudiKernel BcmDetLib)

gaudi_add_dictionary(BcmDet
                     dict/BcmDetDict.h
                     dict/BcmDetDict.xml
                     LINK_LIBRARIES DetDescLib DetDescCnvLib GaudiKernel BcmDetLib
                     OPTIONS "-U__MINGW32__")
