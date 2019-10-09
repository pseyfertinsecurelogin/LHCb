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
# Package: L0Event
################################################################################
gaudi_subdir(L0Event)

gaudi_depends_on_subdirs(Kernel/LHCbKernel)

find_package(Boost)



gaudi_add_library(L0Event
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost LHCbKernel)

gaudi_add_dictionary(L0Event dict/dictionary.h dict/selection.xml LINK_LIBRARIES Boost L0Event LHCbKernel INCLUDE_DIRS Boost)

