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
# Package: EventAssoc
################################################################################
gaudi_subdir(EventAssoc v5r6)

gaudi_depends_on_subdirs(Event/GenEvent
                         Event/L0Event
                         Event/MCEvent
                         Event/PhysEvent
                         Event/TrackEvent
                         GaudiPython
                         Kernel/Relations)

gaudi_add_dictionary(EventAssocPhys
                     dict/selPhys.h
                     dict/selPhys.xml
                     LINK_LIBRARIES GenEvent L0Event MCEvent PhysEvent TrackEvent RelationsLib
                     OPTIONS "-U__MINGW32__")

gaudi_add_dictionary(EventAssocMC
                     dict/selMC.h
                     dict/selMC.xml
                     LINK_LIBRARIES GenEvent L0Event MCEvent PhysEvent TrackEvent RelationsLib
                     OPTIONS "-U__MINGW32__")

