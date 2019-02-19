/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef OTDAQ_OTCHANNELMAP_H_
#define OTDAQ_OTCHANNELMAP_H_

#include "Kernel/OTChannelID.h"
#include "OTDAQ/OTEnum.h"
#include <array>
#include <vector>

class OTChannelMapTool;

namespace OTDAQ {

  enum EModuleLayout {
    OddLayerPosY = 0,
    OddLayerNegY,
    EvenLayerPosY,
    EvenLayerNegY,
    OddLayerPosYShort,
    OddLayerNegYShort,
    EvenLayerPosYShort,
    EvenLayerNegYShort,
    NumDefaultModuleLayouts
  };

  namespace ChannelMap {

    // channel = 32 * otis + channelInOtis

    class Module final {
    public:
      enum { NumStraws = OTDAQ::NumStraws, NumChannels = NumStraws };
      Module();
      unsigned int straw( unsigned int channel ) const { return m_channelToStraw[channel]; }
      unsigned int channel( unsigned int straw ) const { return m_strawToChannel[straw - OffsetStraws]; }
      friend class ::OTChannelMapTool;

    private:
      void                                 fillStrawToChannelMap();
      std::array<unsigned char, NumStraws> m_channelToStraw;
      std::array<unsigned char, NumStraws> m_strawToChannel;
      unsigned char                        m_nstraws = 0;
    };

    inline Module::Module() {
      m_strawToChannel.fill( 0xFF );
      m_channelToStraw.fill( 0xFF );
    }

    inline void Module::fillStrawToChannelMap() {
      // Fills the inverse map
      m_strawToChannel.fill( 0u );
      for ( unsigned char ichan = 0; ichan < NumStraws; ++ichan )
        if ( m_channelToStraw[ichan] != 0xFF ) m_strawToChannel[m_channelToStraw[ichan] - OffsetStraws] = ichan;
    }

    class Detector {
    public:
      Detector() : m_modules( NumDefaultModuleLayouts ) {}

      const Module& module( const LHCb::OTChannelID& modid ) const {
        return module( modid.station(), modid.layer(), modid.quarter(), modid.module() );
      }

      const Module& module( unsigned int station, unsigned int layer, unsigned int quadrant,
                            unsigned int module ) const {
        return m_modules[layout( station, layer, quadrant, module )];
      };
      unsigned int straw( unsigned int station, unsigned int layer, unsigned int quadrant, unsigned int moduleid,
                          unsigned int channelid ) const {
        return module( station, layer, quadrant, moduleid ).straw( channelid );
      }
      unsigned int channel( unsigned int station, unsigned int layer, unsigned int quadrant, unsigned int moduleid,
                            unsigned int strawid ) const {
        return module( station, layer, quadrant, moduleid ).channel( strawid );
      }

      unsigned char layout( unsigned int station, unsigned int layer, unsigned int quadrant,
                            unsigned int module ) const {
        return m_modulepointers[station - OffsetStations][layer - OffsetLayers][quadrant - OffsetQuadrants]
                               [module - OffsetModules];
      };

      unsigned char layout( const LHCb::OTChannelID& modid ) const {
        return layout( modid.station(), modid.layer(), modid.quarter(), modid.module() );
      };

    private:
      friend class ::OTChannelMapTool;

      Module& module( unsigned int station, unsigned int layer, unsigned int quadrant, unsigned int module ) {
        return m_modules[m_modulepointers[station - OffsetStations][layer - OffsetLayers][quadrant - OffsetQuadrants]
                                         [module - OffsetModules]];
      }

      void setModuleLayout( const LHCb::OTChannelID& modid, unsigned char layout ) {
        m_modulepointers[modid.station() - OffsetStations][modid.layer() - OffsetLayers]
                        [modid.quarter() - OffsetQuadrants][modid.module() - OffsetModules] = layout;
      }

    private:
      unsigned char       m_modulepointers[NumStations][NumLayers][NumQuadrants][NumModules];
      std::vector<Module> m_modules;
    };
  } // namespace ChannelMap
} // namespace OTDAQ

#endif
