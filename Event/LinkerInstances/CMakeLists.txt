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
# Package: LinkerInstances
################################################################################
gaudi_subdir(LinkerInstances)

gaudi_depends_on_subdirs(Event/DigiEvent
                         Event/FTEvent
                         Event/LinkerEvent
                         Event/MCEvent
                         Event/PhysEvent
                         Event/RecEvent
                         Event/TrackEvent)

gaudi_add_dictionary(LinkerInstances
                     dict/linker_instances.h
                     xml/lcg_selection.xml
                     LINK_LIBRARIES LinkerEvent MCEvent PhysEvent RecEvent TrackEvent
                     OPTIONS "-U__MINGW32__")

gaudi_install_python_modules()

