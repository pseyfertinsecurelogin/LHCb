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

###############################################################################
# Package: RichUtils
###############################################################################

gaudi_subdir(RichUtils)

gaudi_depends_on_subdirs(Event/DAQEvent
	                 GaudiUtils
                         Kernel/LHCbKernel)

find_package(AIDA)
find_package(Boost)
find_package(VDT)
find_package(Eigen)
find_package(Vc)

find_path(RANGES_V3_INCLUDE_DIR NAMES range/v3/all.hpp)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${EIGEN_INCLUDE_DIRS} ${Vc_INCLUDE_DIR})

gaudi_add_library(RichUtils
                  src/*.cpp
                  PUBLIC_HEADERS RichUtils
                  INCLUDE_DIRS Vc Eigen AIDA Boost VDT Kernel/LHCbKernel Event/DAQEvent ${RANGES_V3_INCLUDE_DIR}
                  LINK_LIBRARIES Boost VDT DAQEventLib GaudiUtilsLib LHCbKernel)

gaudi_add_dictionary(RichUtils
                     dict/RichUtilsDict.h
                     dict/RichUtilsDict.xml
                     INCLUDE_DIRS VDT Vc Eigen AIDA Boost
                     LINK_LIBRARIES VDT Boost DAQEventLib GaudiUtilsLib LHCbKernel RichUtils
                     OPTIONS " -Wno-undefined-var-template -U__MINGW32__ ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

target_link_libraries( RichUtils "${Vc_LIB_DIR}/libVc.a" )
