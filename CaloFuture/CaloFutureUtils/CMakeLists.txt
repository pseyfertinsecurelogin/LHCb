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
# Package: CaloFutureUtils
################################################################################
gaudi_subdir(CaloFutureUtils)

gaudi_depends_on_subdirs(CaloFuture/CaloFutureInterfaces
                         Det/CaloDet
                         Event/DigiEvent
                         Event/L0Event
                         Event/MCEvent
                         Event/PhysEvent
                         Event/RecEvent
                         GaudiAlg
                         Kernel/LHCbMath)

# hide warnings from some external projects
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

find_package(AIDA)

gaudi_add_library(CaloFutureUtils
                  src/*.cpp
                  PUBLIC_HEADERS CaloFutureUtils
                  INCLUDE_DIRS AIDA Event/DigiEvent
                  LINK_LIBRARIES CaloDetLib L0Event MCEvent PhysEvent RecEvent GaudiAlgLib LHCbMathLib)

gaudi_add_dictionary(CaloFutureUtils
                     dict/CaloFutureUtilsDict.h
                     dict/CaloFutureUtils.xml
                     INCLUDE_DIRS Event/DigiEvent
                     LINK_LIBRARIES CaloDetLib L0Event MCEvent PhysEvent RecEvent GaudiAlgLib LHCbMathLib CaloFutureUtils
                     OPTIONS " -U__MINGW32__ ")

gaudi_install_python_modules()

