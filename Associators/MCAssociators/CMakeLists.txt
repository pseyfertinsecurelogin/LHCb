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
# Package: MCAssociators
################################################################################
gaudi_subdir(MCAssociators)

gaudi_depends_on_subdirs(Event/LinkerEvent
                         Event/MCEvent
                         Event/PhysEvent
                         GaudiAlg
                         GaudiKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})


gaudi_add_dictionary(MCAssociators
                     dict/MCAssociatorsDict.h
                     dict/MCAssociatorsDict.xml
                     LINK_LIBRARIES LinkerEvent MCEvent PhysEvent GaudiAlgLib GaudiKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(Kernel)

