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
# Package: CaloDet
################################################################################
gaudi_subdir(CaloDet)

gaudi_depends_on_subdirs(Calo/CaloKernel
                         Det/DetDesc
                         GaudiAlg
                         Kernel/LHCbKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_library(CaloDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS CaloDet
                  INCLUDE_DIRS Calo/CaloKernel
                  LINK_LIBRARIES DetDescLib GaudiAlgLib LHCbKernel)

gaudi_add_module(CaloDet
                 src/component/*.cpp
                 INCLUDE_DIRS Calo/CaloKernel
                 LINK_LIBRARIES DetDescLib GaudiAlgLib LHCbKernel CaloDetLib)

gaudi_add_dictionary(CaloDet
                     dict/CaloDetDict.h
                     dict/CaloDetDict.xml
                     INCLUDE_DIRS Calo/CaloKernel
                     LINK_LIBRARIES DetDescLib GaudiAlgLib LHCbKernel CaloDetLib
                     OPTIONS "-U__MINGW32__")

gaudi_env(SET CALODETOPTS \${CALODETROOT}/options)

