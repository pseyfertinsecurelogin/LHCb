###############################################################################
# (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
# Module locating the VectorClass headers.
#
# Defines:
#  - VectorClass_FOUND
#  - VectorClass_INCLUDE_DIR
#  - VectorClass_INCLUDE_DIRS (not cached)

# Look for the header directory:
find_path( VectorClass_INCLUDE_DIR
   NAMES vectorclass.h
   HINTS $ENV{VectorClass_ROOT_DIR}/include ${VectorClass_ROOT_DIR}/include )
set( VectorClass_INCLUDE_DIRS ${VectorClass_INCLUDE_DIR} )

# Handle the regular find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( VectorClass DEFAULT_MSG VectorClass_INCLUDE_DIR )

# Mark the cached variables as "advanced":
mark_as_advanced( VectorClass_FOUND VectorClass_INCLUDE_DIR )
