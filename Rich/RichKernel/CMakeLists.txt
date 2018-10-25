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
# Package: RichKernel
################################################################################
gaudi_subdir(RichKernel v8r0)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichInterfaces)

#message(STATUS "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
find_package(AIDA)
find_package(Boost)
find_package(VDT)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_library(RichKernelLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS RichKernel
                  INCLUDE_DIRS AIDA Boost VDT RichUtils LHCbKernel
                  LINK_LIBRARIES Boost VDT GaudiAlgLib GaudiUtilsLib RichUtils LHCbKernel)

gaudi_add_module(RichKernel
                 src/component/*.cpp
                 INCLUDE_DIRS AIDA Boost RichUtils LHCbKernel
                 LINK_LIBRARIES Boost GaudiAlgLib GaudiUtilsLib LHCbKernel RichUtils RichKernelLib)

gaudi_install_python_modules()
