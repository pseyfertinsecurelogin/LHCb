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
# Package: SwimmingEvent
################################################################################
gaudi_subdir(SwimmingEvent)

gaudi_depends_on_subdirs(Event/PhysEvent
                         GaudiKernel
                         Kernel/Relations)

gaudi_add_dictionary(SwimmingEvent
                     dict/SwimmingEvent.h
                     dict/SwimmingEvent.xml
                     LINK_LIBRARIES PhysEvent GaudiKernel RelationsLib
                     OPTIONS "--no_membertypedefs")

gaudi_install_headers(Event)

