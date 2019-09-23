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
// Include files

// from Gaudi
#include "CaloDet/DeCalorimeter.h"
#include "GaudiAlg/FunctionalUtilities.h"
// local
#include "CaloFutureDigitFilterAlg.h"
#include "CaloFutureUtils/CaloFutureAlgUtils.h"

// Declaration of the Algorithm Factory

DECLARE_COMPONENT_WITH_ID( LHCb::Calo::DAQ::Algorithms::DigitFilter, "CaloFutureDigitFilterAlg" )

namespace LHCb::Calo::DAQ::Algorithms {

  //-----------------------------------------------------------------------------
  // Implementation file for class : DigitFilter
  //
  // 2010-12-21 : Olivier Deschamps
  //-----------------------------------------------------------------------------

  namespace {
    struct CondValue {
      double offset, offsetRMS;
    };
    using Offsets  = std::map<CaloCellID, double>;
    using CondMaps = std::map<CaloCellID, CondValue>;

    CondValue getOffset( CaloCellID id, int scale, double scalingMin, int scalingBin, const CondMaps& table ) {
      double     offset    = 0;
      double     offsetRMS = 0;
      const auto it        = table.find( id );
      if ( !( ( 0 == scale ) || ( scale <= scalingMin ) || ( it == table.end() ) ) ) {
        const auto& ref = it->second;
        // (approximate) overlap probability
        constexpr double ncells = 6016.;
        double           rscale = scale / ( 1. - scale / ncells );
        double           aa     = 4 * scale / ncells;
        if ( aa < 1 ) { rscale = 0.5 * ncells * ( 1. - std::sqrt( 1. - aa ) ); }
        double aaa = ( rscale - scalingMin ) / double( scalingBin );
        offset     = aaa * ref.offset;
        offsetRMS  = std::sqrt( aaa ) * ref.offset;
      }
      return {offset, offsetRMS};
    }

    //-----------------------
    CondMaps createMaps( const DeCalorimeter& calo ) {

      CondMaps maps;
      // fill the maps from the CaloFuture DetElem
      for ( auto& c : calo.cellParams() ) {
        const auto id = c.cellID();
        if ( !calo.valid( id ) || id.isPin() ) continue;
        maps.emplace( id, CondValue{c.pileUpOffset(), c.pileUpOffsetRMS()} );
      }
      return maps;
    }

  } // namespace

  //=============================================================================
  // Main execution
  //=============================================================================
  DigitFilter::DigitFilter( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer(
            name, pSvcLocator,
            {KeyValue{"InputDigitsLocation",
                      CaloFutureAlgUtils::CaloFutureUnfilteredDigitLocation( name.substr( 0, 4 ) )},
             KeyValue{"PVLocation", RecVertexLocation::Primary},
             KeyValue{"DetectorLocation", LHCb::CaloFutureAlgUtils::DeCaloFutureLocation( name.substr( 0, 4 ) )}},
            KeyValue{"FilteredDigitsLocation", CaloFutureAlgUtils::CaloFutureDigitLocation( name.substr( 0, 4 ) )} ) {}

  //=============================================================================
  // Main execution
  //=============================================================================
  CaloDigits DigitFilter::operator()( const CaloDigits& caloDigits, const RecVertices& verts,
                                      const DeCalorimeter& calo ) const {

    CaloDigits digits;
    for ( const auto& caloDigit : caloDigits ) {
      if ( !caloDigit ) continue;
      auto digit = std::make_unique<CaloDigit>( caloDigit->cellID(), caloDigit->e() );
      digits.insert( digit.get() );
      digit.release();
    }

    constexpr int  val    = 0x3;
    constexpr bool substr = ( val & 0x2 ) != 0;
    constexpr bool mask   = ( val & 0x1 ) != 0;
    if ( !cleanDigits( digits, verts, calo, substr, mask ) ) ++m_filterfailed;

    return digits;
  }

  bool DigitFilter::cleanDigits( const CaloDigits& digits, const RecVertices& verts, const DeCalorimeter& calo,
                                 bool substr, bool mask ) const {

    auto indx = calo.index();
    assert( indx == CaloCellCode::CaloIndex::EcalCalo || indx == CaloCellCode::CaloIndex::HcalCalo );

    std::string det = ( indx == CaloCellCode::CaloIndex::EcalCalo ? "Ecal" : "Hcal" );
    auto        it  = m_maskMap.find( det );
    if ( it != m_maskMap.end() ) return it->second;
    it         = m_maskMap.find( "Default" );
    int m_mask = it != m_maskMap.end() ? it->second : 0;

    int    scalingMethod = m_scalingMethod;
    int    scalingBin    = m_scalingBin;
    double scalingMin    = m_scalingMin;

    if ( m_useCondDB ) {
      scalingMethod = calo.pileUpSubstractionMethod();
      scalingMin    = calo.pileUpSubstractionMin();
      scalingBin    = calo.pileUpSubstractionBin();
    }

    int scale = ( ( scalingMethod == 1 || scalingMethod == 11 ) ? verts.size() : 0 ); // nPV
    m_offsetScaleCounter += scale;

    // need to define "Offsets & table"
    const CondMaps table = createMaps( calo );
    int            nMask = 0;
    int            mOffs = 0;
    int            nOffs = 0;
    for ( CaloDigit* digit : digits ) {

      CaloCellID id = digit->cellID();

      // apply mask
      if ( mask && m_mask != 0 && calo.hasQuality( id, (CaloCellQuality::Flag)m_mask ) ) {
        digit->setE( 0. );
        ++nMask;
      }

      // apply offset to channel (method < 10)
      if ( scalingMethod < 10 && substr && !table.empty() ) {
        double offset = getOffset( id, scale, scalingMin, scalingBin, table ).offset;
        if ( 0. != offset ) {
          digit->setE( std::max( digit->e() - offset, 0. ) );
          mOffs += offset / calo.cellGain( id ); // offset (in ADC)
        }
      }
      ++nOffs;
    }

    if ( nMask != 0 ) m_maskedDigitsCounter += nMask;
    if ( scalingMethod < 10 && substr && !table.empty() ) {
      double ave = ( nOffs != 0 ) ? mOffs / double( nOffs ) : 0;
      m_avgOffsetCounter += -ave;
    }

    return true;
  }

} // namespace LHCb::Calo::DAQ::Algorithms
//=============================================================================
