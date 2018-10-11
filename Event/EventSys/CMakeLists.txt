###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: EventSys
################################################################################
gaudi_subdir(EventSys v26r2)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/DigiEvent
                         Event/EventAssoc
                         Event/EventBase
                         Event/GenEvent
                         Event/HltEvent
                         Event/L0Event
                         Event/LinkerEvent
                         Event/LumiEvent
                         Event/MCEvent
                         Event/PhysEvent
                         Event/RecEvent
                         Event/TrackEvent
                         Event/VeloEvent)

find_package(RELAX REQUIRED COMPONENTS CLHEP HepMC STL Math)

# Merge the RELAX rootmaps
set(rootmapfile ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/relax.rootmap)
add_custom_command(OUTPUT ${rootmapfile}
                   COMMAND ${default_merge_cmd} ${RELAX_ROOTMAPS} ${rootmapfile}
                   DEPENDS ${RELAX_ROOTMAPS})
add_custom_target(RelaxRootmap ALL DEPENDS ${rootmapfile})

# Install the merged file
install(FILES ${rootmapfile} DESTINATION lib)

