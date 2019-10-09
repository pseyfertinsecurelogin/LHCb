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
# Package: DigiEvent
################################################################################
gaudi_subdir(DigiEvent)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost)



gaudi_add_dictionary(DigiEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES Boost LHCbKernel LHCbMathLib INCLUDE_DIRS Boost)

gaudi_install_headers(Event)

