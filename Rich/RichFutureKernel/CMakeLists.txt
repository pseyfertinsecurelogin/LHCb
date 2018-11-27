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
# Package: RichFutureKernel
################################################################################

gaudi_subdir(RichFutureKernel v1r0)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichInterfaces)

find_package(AIDA)
find_package(Boost)
find_package(VDT)
find_package(Vc)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${Vc_INCLUDE_DIR})     

gaudi_add_library(RichFutureKernel
                  src/*.cpp
                  PUBLIC_HEADERS RichFutureKernel
                  INCLUDE_DIRS AIDA Boost VDT RichUtils LHCbKernel
                  LINK_LIBRARIES Boost VDT GaudiAlgLib GaudiUtilsLib RichUtils LHCbKernel)

gaudi_install_python_modules()
