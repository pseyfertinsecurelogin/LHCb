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
# Package: STTELL1Event
################################################################################
gaudi_subdir(STTELL1Event)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         ST/STKernel)

gaudi_install_headers(Event)

gaudi_add_library(STTELL1Event
                  src/*.cpp
                  NO_PUBLIC_HEADERS
                  LINK_LIBRARIES LHCbKernel STKernelLib)

gaudi_add_dictionary(STTELL1Event dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel STKernelLib STTELL1Event)

