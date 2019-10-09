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
# Package: FTEvent
################################################################################
gaudi_subdir(FTEvent)

gaudi_depends_on_subdirs(Event/EventBase
                         Event/MCEvent
                         Kernel/LHCbKernel)

gaudi_install_headers(Event)

gaudi_add_dictionary(FTEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel MCEvent INCLUDE_DIRS Event/EventBase)

