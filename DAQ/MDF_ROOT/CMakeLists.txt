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
# Package: MDF_ROOT
################################################################################
gaudi_subdir(MDF_ROOT v1r9)

gaudi_depends_on_subdirs(DAQ/MDF)

find_package(ROOT COMPONENTS RIO)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_add_library(MDF_ROOT
                  src/*.cpp
                  NO_PUBLIC_HEADERS
                  INCLUDE_DIRS ROOT
                  LINK_LIBRARIES ROOT MDFLib)

