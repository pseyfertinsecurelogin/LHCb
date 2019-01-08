###############################################################################
# (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: VectorClass
################################################################################
gaudi_subdir(VectorClass)

find_package(VectorClass REQUIRED)

gaudi_add_library(VectorClassLib
                  ${VectorClass_INCLUDE_DIR}/ranvec1.cpp
                  ${VectorClass_INCLUDE_DIR}/instrset_detect.cpp
                  NO_PUBLIC_HEADERS
                  INCLUDE_DIRS VectorClass)

# Disable some warnings
set_property(SOURCE ${VectorClass_INCLUDE_DIR}/ranvec1.cpp
             PROPERTY COMPILE_FLAGS " -Wno-overflow -Wno-sign-compare -Wno-strict-aliasing " )
