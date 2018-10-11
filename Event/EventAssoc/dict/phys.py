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
# =============================================================================
## @file
#  Simple script to create proper selection files to buils relations
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#  @date   2006-11-26
# =============================================================================
""" Simple script to create proper selection files to buils relations """
# =============================================================================
__author__ = 'Vanya BELYAEV ibelyaev@physics.syr.edu'

import Relations.Rels as Rels 
    
classes  = []
includes = []
    
classes += [ Rels.RelW2D( 'LHCb::CaloHypo'      , 'LHCb::Track'       , 'float'  ) ]
classes += [ Rels.RelW2D( 'LHCb::CaloCluster'   , 'LHCb::Track'       , 'float'  ) ]
classes += [ Rels.Rel1D ( 'LHCb::Track'                               , 'float'  ) ]
classes += [ Rels.RelW1D( 'LHCb::Particle'      , 'LHCb::VertexBase'  , 'double' ) ]
classes += [ Rels.Rel1D( 'LHCb::Particle'       , 'LHCb::VertexBase'             ) ]
classes += [ Rels.Rel1D( 'LHCb::Particle'       , 'int'             ) ]
classes += [ Rels.Rel1D( 'LHCb::Particle'       , 'double'          ) ]
classes += [ Rels.Rel1D( 'LHCb::Particle'       , 'LHCb::RelatedInfoMap'  ) ]
classes += [ Rels.RelW1D( 'LHCb::Particle' , 'LHCb::Particle'  , 'double' ) ]

includes += ['Event/Particle.h'      ]
includes += ['Event/Track.h'         ]
includes += ['Event/CaloCluster.h'   ]
includes += ['Event/CaloHypo.h'      ]
includes += ['Event/VertexBase.h'    ]
includes += ['Event/RelatedInfoMap.h']

__all__ = ( classes, includes )

# =============================================================================
# The END 
# =============================================================================
