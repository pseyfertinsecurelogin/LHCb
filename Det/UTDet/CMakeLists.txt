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
# Package: UTDet
################################################################################
gaudi_subdir(UTDet v1r0)

gaudi_depends_on_subdirs(Det/DetDesc
                         Det/DetDescCnv
                         Kernel/LHCbKernel)

find_package(Boost)

gaudi_add_library(UTDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS UTDet
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel)

gaudi_add_module(UTDet
                 src/Component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel UTDetLib)

gaudi_add_dictionary(UTDet
                     dict/UTDetDict.h
                     dict/UTDetDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel UTDetLib
                     OPTIONS "-U__MINGW32__")

