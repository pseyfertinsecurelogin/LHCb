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
# Package: VPDet
################################################################################
gaudi_subdir(VPDet)

gaudi_depends_on_subdirs(Det/DetDesc
                         Det/DetDescCnv
                         GaudiAlg
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost)

gaudi_add_library(VPDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS VPDet
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib)

gaudi_add_module(VPDet
                 src/component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib VPDetLib)

gaudi_add_dictionary(VPDet
                     dict/VPDetDict.h
                     dict/VPDetDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib VPDetLib
                     OPTIONS "-U__MINGW32__")

