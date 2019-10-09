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
# Package: RecEvent
################################################################################
gaudi_subdir(RecEvent)

gaudi_depends_on_subdirs(Event/DigiEvent
                         Event/EventBase
                         Event/TrackEvent
                         Kernel/LHCbKernel
                         Kernel/LHCbMath
                         Kernel/PartProp
                         Kernel/SOAContainer
                         Kernel/SOAExtensions)

gaudi_add_library(RecEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Event/DigiEvent Event/EventBase Kernel/SOAContainer Kernel/SOAExtensions
                  LINK_LIBRARIES TrackEvent LHCbMathLib PartPropLib)

gaudi_add_unit_test(test_CaloHypo_v2 tests/src/test_CaloHypo_v2.cpp
   LINK_LIBRARIES GaudiKernel TrackEvent RecEvent
                    TYPE Boost)

gaudi_add_dictionary(RecEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel LHCbMathLib PartPropLib RecEvent TrackEvent INCLUDE_DIRS Event/DigiEvent Event/EventBase Kernel/SOAContainer)

gaudi_add_unit_test(TestMuonZipInfrastructure
                    tests/src/TestMuonZipInfrastructure.cpp
                    LINK_LIBRARIES LHCbMathLib
                    TYPE Boost)
