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
# Package: FSREvent
################################################################################
gaudi_subdir(FSREvent)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel)

find_package(Boost)

gaudi_install_headers(Event)

gaudi_add_dictionary(FSREvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES Boost GaudiKernel LHCbKernel INCLUDE_DIRS Boost)

