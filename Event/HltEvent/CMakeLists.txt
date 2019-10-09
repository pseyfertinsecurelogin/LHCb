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
# Package: HltEvent
################################################################################
gaudi_subdir(HltEvent)

gaudi_depends_on_subdirs(Event/PhysEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

gaudi_install_headers(Event)

gaudi_add_library(HltEvent
                  src/*.cpp
                  NO_PUBLIC_HEADERS
                  LINK_LIBRARIES PhysEvent GaudiKernel)

gaudi_add_dictionary(HltEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES GaudiKernel HltEvent LHCbKernel PhysEvent)

