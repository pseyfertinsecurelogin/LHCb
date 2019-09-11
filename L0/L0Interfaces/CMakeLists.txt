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
# Package: L0Interfaces
################################################################################
gaudi_subdir(L0Interfaces)

gaudi_depends_on_subdirs(Event/L0Event)

gaudi_add_dictionary(L0Interfaces
                     dict/L0InterfacesDict.h
                     dict/L0InterfacesDict.xml
                     LINK_LIBRARIES L0Event
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(L0Interfaces)

