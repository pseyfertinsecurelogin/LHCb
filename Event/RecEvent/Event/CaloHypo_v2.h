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

#pragma once

// Include files

#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloPosition.h"
#include "Kernel/CaloCellID.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include "Kernel/meta_enum.h"
#include <memory>
#include <optional>
#include <utility>
#include <vector>

/**
 *
 * CaloHypo v2 is the base class for offline and online CaloHypos.
 *
 * @author Zehua Xu, zehua.xu@cern.ch
 *
 */

namespace LHCb::Event {
  namespace v2 {

    // Namespace for locations in TDS
    namespace CaloHypoLocation {
      inline const std::string Default         = "Rec/Calo/Hypos";
      inline const std::string Photons         = "Rec/Calo/Photons";
      inline const std::string Electrons       = "Rec/Calo/Electrons";
      inline const std::string MergedPi0s      = "Rec/Calo/MergedPi0s";
      inline const std::string SplitPhotons    = "Rec/Calo/SplitPhotons";
      inline const std::string DefaultHlt      = "Hlt/Calo/Hypos";
      inline const std::string PhotonsHlt      = "Hlt/Calo/Photons";
      inline const std::string PhotonsHlt1     = "Hlt1/Calo/Photons";
      inline const std::string ElectronsHlt    = "Hlt/Calo/Electrons";
      inline const std::string MergedPi0sHlt   = "Hlt/Calo/MergedPi0s";
      inline const std::string SplitPhotonsHlt = "Hlt/Calo/SplitPhotons";
    } // namespace CaloHypoLocation

    //
    namespace Enum::CaloHypo {

      /// CaloHypo type enumerations
      meta_enum_class( Hypothesis, int, Undefined, Unknown, Mip, MipPositive, MipNegative, Photon, PhotonFromMergedPi0,
                       BremmstrahlungPhoton, Pi0Resolved, Pi0Overlapped, Pi0Merged, EmCharged, Positron, Electron,
                       EmChargedSeed, PositronSeed, ElectronSeed, NeutralHadron, ChargedHadron, PositiveHadron,
                       NegativeHadron, Jet, Other )
    } // namespace Enum::CaloHypo

    /// Reconstructed CaloHypo class
    class CaloHypo final {

    public:
      using Hypothesis = Enum::CaloHypo::Hypothesis;

      /// Shortcut for the type of CaloPosition object
      typedef LHCb::CaloPosition Position;

      /// Constructor
      CaloHypo( Hypothesis hypothesis, double lh, Position pos, std::vector<CaloDigit> digits,
                std::vector<const CaloCluster*> clusters, std::vector<const CaloHypo*> hypos ) noexcept
          : m_hypothesis( hypothesis )
          , m_lh( lh )
          , m_position( std::move( pos ) )
          , m_digits( std::move( digits ) )
          , m_clusters( std::move( clusters ) )
          , m_hypos( std::move( hypos ) ) {}

      /// Constructor
      CaloHypo( Hypothesis hypothesis, double lh, Position pos, std::vector<CaloDigit> digits,
                std::vector<const CaloCluster*> clusters ) noexcept
          : m_hypothesis( hypothesis )
          , m_lh( lh )
          , m_position( std::move( pos ) )
          , m_digits( std::move( digits ) )
          , m_clusters( std::move( clusters ) ) {}

      /// Default Constructor
      CaloHypo() = default;

      /// Print this CaloHypo data object in a human readable way
      std::ostream& fillStream( std::ostream& s ) const;

      /// Retrieve const  The hypothesis's ID
      const Hypothesis& hypothesis() const { return m_hypothesis; }

      /// Update  The hypothesis's ID
      void setHypothesis( const Hypothesis& value ) { m_hypothesis = value; }

      /// Retrieve const  The Hypothesis's likelihood
      float likelihood() const { return m_lh; }

      /// Update  The Hypothesis's likelihood
      void setLikelihood( const double& value ) { m_lh = value; };

      /// Retrieve const  The hypothesis's CaloPosition object
      const auto& position() const { return m_position; }

      /// Update the hypothesis's CaloPosition object.
      void setPosition( Position pos ) { m_position = std::move( pos ); }

      // /// Retrieve the energy @attention it is just a shortcut!
      // double e() const { return m_position.e() ? m_position.e() : 0; }
      double e() const { return m_position.e(); }

      // retrive the digits
      const std::vector<CaloDigit>& digits() const { return m_digits; }
      std::vector<CaloDigit>&       digits() { return m_digits; }

      /// reserve space for n CaloDigits
      void reserveToDigits( unsigned int n = 9 ) {
        m_digits.clear();
        m_digits.reserve( n );
      }

      // Add to the Calorimeter digit
      void addToDigits( CaloDigit value ) { m_digits.emplace_back( std::move( value ) ); }

      /// Retrieve const the CaloCluster object
      const std::vector<const CaloCluster*>& clusters() const { return m_clusters; }

      /// Update the hypothesis's CaloClusterr object.
      void setClusters( CaloCluster* value ) { m_clusters.emplace_back( value ); }

      // retrieve the hypos for merged \piz
      const std::vector<const CaloHypo*>& hypos() const { return m_hypos; }

      /// reserve space for n CaloHypos
      void reserveToHypos( unsigned int n = 2 ) {
        m_hypos.clear();
        m_hypos.reserve( n );
      }

      // Add to the Calorimeter Hypos
      void addToHypos( CaloHypo* value ) { m_hypos.emplace_back( value ); }

    private:
      Hypothesis                      m_hypothesis = Hypothesis::Undefined;
      float                           m_lh         = -1;
      Position                        m_position;
      std::vector<CaloDigit>          m_digits;   // To retrieve digits
      std::vector<const CaloCluster*> m_clusters; // To retrieve clusters
      std::vector<const CaloHypo*>    m_hypos;    // Designed for Merged Pi0

    }; // Class CaloHypo

    inline std::ostream& LHCb::Event::v2::CaloHypo::fillStream( std::ostream& s ) const {
      s << "{\n"
        << "hypothesis : " << m_hypothesis << '\n'
        << "lh : " << m_lh << '\n';
      // if ( m_position.has_value() ) {
      //  s << "position : " << m_position.value();
      // if ( m_position.has_value() ) {
      //  s << "position : " << m_position.value().e();
      //} else {
      //  s << "No position";
      //}
      return s << "\n }";
    } // inline override function for debug

  } // namespace v2
} // namespace LHCb::Event
