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
# Package: VectorClass
################################################################################
gaudi_subdir(VectorClass v1r30)

gaudi_depends_on_subdirs(GaudiPolicy)

gaudi_add_library(VectorClassLib
                  src/*.cpp
                  PUBLIC_HEADERS VectorClass
                  INCLUDE_DIRS VectorClass)

# Disable some warnings
set_property(SOURCE src/ranvec1.cpp PROPERTY COMPILE_FLAGS " -Wno-overflow -Wno-sign-compare -Wno-strict-aliasing " )
