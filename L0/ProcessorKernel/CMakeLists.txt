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
# Package: ProcessorKernel
################################################################################
gaudi_subdir(ProcessorKernel)

gaudi_depends_on_subdirs(Kernel/LHCbKernel)

find_package(Boost)
find_package(XercesC)

gaudi_add_library(ProcessorKernel
                  src/*.cpp
                  PUBLIC_HEADERS ProcessorKernel
                  INCLUDE_DIRS Boost XercesC
                  LINK_LIBRARIES Boost XercesC LHCbKernel)

