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
# Package: VeloEvent
################################################################################
gaudi_subdir(VeloEvent)

gaudi_depends_on_subdirs(DAQ/Tell1Kernel
                         Kernel/LHCbKernel)

gaudi_install_headers(VeloEvent)

gaudi_add_dictionary(VeloEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel INCLUDE_DIRS DAQ/Tell1Kernel)
