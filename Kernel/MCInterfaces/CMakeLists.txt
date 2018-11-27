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
# Package: MCInterfaces
################################################################################
gaudi_subdir(MCInterfaces v1r19)

gaudi_depends_on_subdirs(Event/GenEvent
                         Event/MCEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

gaudi_add_dictionary(MCInterfaces
                     dict/MCInterfacesDict.h
                     dict/MCInterfacesDict.xml
                     LINK_LIBRARIES TrackEvent GenEvent MCEvent GaudiKernel LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(MCInterfaces)

