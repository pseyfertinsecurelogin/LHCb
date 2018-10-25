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
# Package: HltInterfaces
################################################################################
gaudi_subdir(HltInterfaces v6r6)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/HltEvent
                         GaudiKernel
                         Kernel/LHCbMath)

find_package(Boost COMPONENTS regex)

if(Boost_VERSION STREQUAL "105900")
  # See https://svn.boost.org/trac/boost/ticket/11743
  add_definitions(-UNDEBUG)
endif()

gaudi_add_library(HltInterfaces
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost DAQEventLib HltEvent GaudiKernel LHCbMathLib)

gaudi_add_dictionary(HltInterfaces
                     dict/HltInterfacesDict.h
                     dict/HltInterfacesDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost DAQEventLib HltEvent GaudiKernel LHCbMathLib HltInterfaces
                     OPTIONS "-U__MINGW32__")

