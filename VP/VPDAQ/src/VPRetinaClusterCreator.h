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
#ifndef VPRETINACLUSTERCREATOR_H
#define VPRETINACLUSTERCREATOR_H 1

#include <array>
#include <tuple>
#include <vector>

// Gaudi
#include "GaudiAlg/Transformer.h"

// LHCb
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/VPLightCluster.h"
#include "Kernel/VPChannelID.h"
#include "Kernel/VPConstants.h"
#include "VPDet/DeVP.h"
#include "VPKernel/PixelModule.h"
#include "VPKernel/VeloPixelInfo.h"

// local
#include "VPRetinaMatrix.h"
#include "VPRetinaCluster.h"

/** @class VPRetinaClusterCreator VPRetinaClusterCreator.h
 * Algorithm to create RetinaCluster Raw Bank from SuperPixel.
 *
 * There is one raw bank per sensor, that is the sensor number (0-207)
 * is the source ID of the bank. Note that this means there is no
 * need to encode the sensor in the RetinaCluster addresses.
 *
 * Each bank has a four byte word header, followed by a four byte
 * RetinaCluster word for each RetinaCluster on the sensor.
 *
 * The header word is currently simply the number of RetinaCluster
 * on the sensor.
 *
 * The RetinaCluster word encoding is the following:
 *
 * bit 0-2    RetinaCluster Fraction Row (0-1 by step of 0.125)
 * bit 3-10   RetinaCluster Row (0-255)
 * bit 11-13  RetinaCluster Fraction Column (0-1 by step of 0.125)
 * bit 14-23  RetinaCluster Column (0-767)
 * bit 24-25  SensorID in the module
 * bit 26-31  UNUSED
 *
 * @author Federico Lazzari
 * @date   2018-06-20
 */


class VPRetinaClusterCreator
  : public Gaudi::Functional::Transformer< LHCb::RawEvent (const LHCb::RawEvent& )>
{

public:
  /// Standard constructor
  VPRetinaClusterCreator( const std::string& name, ISvcLocator* pSvcLocator );

  /// Algorithm initialization
  StatusCode initialize() override;

  /// Algorithm execution
  LHCb::RawEvent operator()( const LHCb::RawEvent& ) const override;

private:

  /// bank version. (change this every time semantics change!)
  const unsigned int m_bankVersion = 1;

  /// make RetinaClusters from bank
  std::vector<uint32_t> makeRetinaClusters(const uint32_t* bank) const;

  /// Recompute the geometry in case of change
  StatusCode rebuildGeometry();
  //
  /// Detector element
  DeVP* m_vp{nullptr};

  /// List of pointers to modules (which contain pointers to their hits)
  std::vector<PixelModule*> m_modules;
  std::vector<PixelModule> m_module_pool;

  /// Indices of first and last module
  unsigned int m_firstModule;
  unsigned int m_lastModule;

  float m_ltg[16 * VP::NSensors]; // 16*208 = 16*number of sensors
  LHCb::span<const double> m_local_x;
  LHCb::span<const double> m_x_pitch;
  float m_pixel_size;
};

#endif // VPRETINACLUSTERCREATOR_H

