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
    std::pair<double, double> getOffset( CaloCellID id, int scale, double scalingMin, int scalingBin,
                                         DigitFilter::CondMaps& table ) {
      double     offset    = 0;
      double     offsetRMS = 0;
      const auto it        = table.find( id );
      if ( !( ( 0 == scale ) || ( scale <= scalingMin ) || ( it == table.end() ) ) ) {
        const auto& ref = it->second;
        //  overlap probabilty (approximate)
        constexpr double ncells = 6016.;
        double           rscale = scale / ( 1. - scale / ncells );
        double           aa     = 4 * scale / ncells;
        if ( aa < 1 ) { rscale = 0.5 * ncells * ( 1. - std::sqrt( 1. - aa ) ); }
        double step = double( scalingBin );
        double bin  = double( rscale - scalingMin );
        double aaa  = bin / step;
        offset      = aaa * ref.offset;
        offsetRMS   = std::sqrt( aaa ) * ref.offset;
      }
      return {offset, offsetRMS};
    }

    //-----------------------
    DigitFilter::CondMaps createMaps( DeCalorimeter& calo ) {

      DigitFilter::CondMaps maps;
      // fill the maps from the CaloFuture DetElem
      for ( auto& c : calo.cellParams() ) {
        const auto id = c.cellID();
        if ( !calo.valid( id ) || id.isPin() ) continue;
        DigitFilter::CondValue val;
        val.offset    = c.pileUpOffset();
        val.offsetRMS = c.pileUpOffsetRMS();
        maps[id]      = val;
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
             KeyValue{"PVLocation", RecVertexLocation::Primary}},
            KeyValue{"FilteredDigitsLocation", CaloFutureAlgUtils::CaloFutureDigitLocation( name.substr( 0, 4 ) )} ) {}

  //=============================================================================
  // Initialization
  //=============================================================================
  StatusCode DigitFilter::initialize() {

    StatusCode sc = Transformer::initialize();
    if ( sc.isFailure() ) return sc;

    // Retrieve the calorimeter we are working with.
    m_calo = getDet<DeCalorimeter>( CaloFutureAlgUtils::DeCaloFutureLocation( m_detectorName.value() ) );
    if ( !m_calo ) { return Error( "Detector information is not available!" ); }

    return StatusCode::SUCCESS;
  }

  //=============================================================================
  // Main execution
  //=============================================================================
  CaloDigits DigitFilter::operator()( const CaloDigits& caloDigits, const RecVertices& recVertices ) const {

    CaloDigits newDigits;
    for ( const auto& caloDigit : caloDigits ) {
      CaloCellID id    = caloDigit->cellID();
      double     e     = caloDigit->e();
      auto       digit = std::make_unique<CaloDigit>( id, e );
      newDigits.insert( digit.get() );
      digit.release();
    }

    int  val    = 0x3;
    bool substr = ( val & 0x2 ) != 0;
    bool mask   = ( val & 0x1 ) != 0;
    if ( m_detectorName == "Ecal" )
      if ( !cleanDigits( newDigits, recVertices, m_detectorName, substr, mask ) )
        Warning( "Ecal digits filtering failed", StatusCode::SUCCESS ).ignore();
    if ( m_detectorName == "Hcal" )
      if ( !cleanDigits( newDigits, recVertices, m_detectorName, substr, mask ) )
        Warning( "Hcal digits filtering failed", StatusCode::SUCCESS ).ignore();

    return newDigits;
  }

  bool DigitFilter::cleanDigits( const CaloDigits& digits, const RecVertices& verts, const std::string& det,
                                 bool substr, bool mask ) const {

    auto it = m_maskMap.find( det );
    if ( it != m_maskMap.end() ) return it->second;
    it         = m_maskMap.find( "Default" );
    int m_mask = ( it != m_maskMap.end() ? it->second : 0 );

    int    scalingMethod = m_scalingMethod;
    int    scalingBin    = m_scalingBin;
    double scalingMin    = m_scalingMin;

    if ( m_useCondDB ) {
      scalingMethod = m_calo->pileUpSubstractionMethod();
      scalingMin    = m_calo->pileUpSubstractionMin();
      scalingBin    = m_calo->pileUpSubstractionBin();
    }

    DigitFilter::CondMaps map = createMaps( *m_calo );

    // if method is nPV, scale with the # of verts
    int scale = ( ( scalingMethod == 1 || scalingMethod == 11 ) ? verts.size() : 0 );
    m_offsetScaleCounter += scale;

    // need to define "Offsets & table"
    // Offsets& table   = map.offsets;
    DigitFilter::CondMaps& table   = map;
    int                    m_nMask = 0;
    int                    m_mOffs = 0.;
    int                    nOffs   = 0;
    for ( CaloDigit* digit : digits ) {

      if ( !digit ) continue;
      CaloCellID id = digit->cellID();

      // apply mask
      if ( mask && m_mask != 0 && m_calo->hasQuality( id, (CaloCellQuality::Flag)m_mask ) ) {
        digit->setE( 0. );
        m_nMask++;
      }

      // apply offset to channel (method < 10)
      if ( scalingMethod < 10 && substr && !table.empty() ) {

        double offset = std::get<0>( getOffset( id, scale, scalingMin, scalingBin, table ) );

        if ( 0. != offset ) {
          digit->setE( std::max( digit->e() - offset, 0. ) );
          m_mOffs += offset / m_calo->cellGain( id ); // offset (in ADC)
        }
      }
      nOffs++;
    }

    if ( m_nMask != 0 ) m_maskedDigitsCounter += m_nMask;
    double ave = ( nOffs != 0 ) ? m_mOffs / double( nOffs ) : 0;
    if ( scalingMethod < 10 && substr && !table.empty() ) m_avgOffsetCounter += -ave;

    return true;
  }

} // namespace LHCb::Calo::DAQ::Algorithms
//=============================================================================
