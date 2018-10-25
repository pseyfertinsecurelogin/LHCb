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
"""
##############################################################################
#                                                                            #
#  Velo/VeloDAQ helper module.                                               #
#  Contains default configurations of the DecodeVeloRawBuffer algorithm      #
#  for decoding into VeloLiteClusters or VeloClusters.                       #
#                                                                            #
#  @package Velo/VeloDAQ                                                     #
#  @author  Eduardo Rodrigues  ( e.rodrigues@physics.gla.ac.uk )             #
#  @date    06/10/2008                                                       #
#                                                                            #
#                                                                            #
##############################################################################
"""

from Configurables import DecodeVeloRawBuffer

# =============================================================================
# Instance of DecodeVeloRawBuffer for decoding into VeloLiteClusters
# =============================================================================
def DefaultDecoderToVeloLiteClusters(
    Name                     = 'DecodeVeloRawBufferToVeloLiteClusters',
    RawEventLocations        = ['Velo/RawEvent', 
                                'Other/RawEvent',
                                'DAQ/RawEvent'],
    VeloClusterLocation      ='Raw/Velo/Clusters',
    DecodeToVeloClusters     = False,
    DecodeToVeloLiteClusters = True
    ):
    
    decoder = DecodeVeloRawBuffer( Name )
    
    decoder.RawEventLocations        = RawEventLocations
    decoder.VeloClusterLocation      = VeloClusterLocation
    decoder.DecodeToVeloClusters     = DecodeToVeloClusters
    decoder.DecodeToVeloLiteClusters = DecodeToVeloLiteClusters
    
    return decoder


# =============================================================================
# Instance of DecodeVeloRawBuffer for decoding into VeloClusters
# =============================================================================
def DefaultDecoderToVeloClusters(
    Name                     = 'DecodeVeloRawBufferToVeloClusters',
    RawEventLocations        = ['Velo/RawEvent',
                                'Other/RawEvent',
                                'DAQ/RawEvent'],
    VeloClusterLocation      ='Raw/Velo/Clusters',
    DecodeToVeloClusters     = True,
    DecodeToVeloLiteClusters = False
    ):
    
    decoder = DecodeVeloRawBuffer( Name )
    
    decoder.RawEventLocations        = RawEventLocations
    decoder.VeloClusterLocation      = VeloClusterLocation
    decoder.DecodeToVeloClusters     = DecodeToVeloClusters
    decoder.DecodeToVeloLiteClusters = DecodeToVeloLiteClusters
    
    return decoder

###############################################################################
