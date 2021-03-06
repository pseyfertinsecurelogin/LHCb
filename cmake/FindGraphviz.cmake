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
# - Locate GRAPHVIZ library
# Defines:
#
#  GRAPHVIZ_FOUND
#  GRAPHVIZ_INCLUDE_DIR
#  GRAPHVIZ_INCLUDE_DIRS (not cached)
#  GRAPHVIZ_LIBRARIES


find_path(GRAPHVIZ_INCLUDE_DIR NAMES graphviz/graph.h
  HINTS ${_GRAPHVIZ_INCLUDE_DIR})

if(GRAPHVIZ_INCLUDE_DIR)
  if(GRAPHVIZ_INCLUDE_DIR MATCHES "include$")
    get_filename_component(_GRAPHVIZROOT ${GRAPHVIZ_INCLUDE_DIR} PATH)
    set(_GRAPHVIZROOT ${_GRAPHVIZROOT} CACHE PATH "Location of the installation of GRAPHVIZ" FORCE)
  endif()
endif()

set(GRAPHVIZ_BINARY_PATH ${_GRAPHVIZROOT}/bin)

find_library(GRAPHVIZ_LIBRARIES NAMES cdt
          HINTS $ENV{GRAPHVIZ_GRAPHVIZ_DIR}/lib ${GRAPHVIZ_GRAPHVIZ_DIR}/lib)

set(GRAPHVIZ_INCLUDE_DIRS ${GRAPHVIZ_INCLUDE_DIR} CACHE PATH "Graphiz bin directory" FORCE)

# handle the QUIETLY and REQUIRED arguments and set GRAPHVIZ_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GRAPHVIZ DEFAULT_MSG GRAPHVIZ_INCLUDE_DIR GRAPHVIZ_LIBRARIES)

mark_as_advanced(GRAPHVIZ_FOUND GRAPHVIZ_INCLUDE_DIR GRAPHVIZ_LIBRARIES GRAPHVIZ_BINARY_PATH)
