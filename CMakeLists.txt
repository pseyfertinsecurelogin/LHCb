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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#---------------------------------------------------------------
# Load macros and functions for Gaudi-based projects
find_package(GaudiProject)
#---------------------------------------------------------------

if(EXISTS ${CMAKE_SOURCE_DIR}/Online/RootCnv)
  set(override_subdirs RootCnv)
endif()

# hide warnings from some external projects
find_package(Boost)
find_package(ROOT)
find_package(CLHEP)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${CLHEP_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

# Declare project name and version
gaudi_project(LHCb v50r2
              USE Gaudi v30r5
              DATA Gen/DecFiles
                   FieldMap
                   ParamFiles
                   PRConfig
                   RawEventFormat
                   TCK/HltTCK
		   TCK/L0TCK VERSION v5r*)
