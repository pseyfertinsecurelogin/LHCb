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
#ifndef EVENT_TURNINGPOINT_H
#define EVENT_TURNINGPOINT_H 1

// Include files
#include <map>
#include <sstream>
#include <string>

// Gaudi
#include <GaudiKernel/DataObject.h>
#include <GaudiKernel/GaudiException.h>
#include <GaudiKernel/Hash.h>

// LHCb Event
#include <Event/Particle.h>
#include <Event/ProtoParticle.h>
#include <Event/Track.h>

#ifdef _WIN32
// Avoid conflict of Windows macro with std::max
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#endif

namespace {
  GaudiUtils::Hash<std::vector<unsigned int>> _hash;
}

namespace Swimming {
  struct Helpers {
    static void decayTree( const LHCb::Particle& p, LHCb::Particle::ConstVector& particles ) {
      particles.push_back( &p );
      if ( !p.isBasicParticle() ) {
        typedef SmartRefVector<LHCb::Particle> Daughters;
        const Daughters&                       daughters = p.daughters();
        for ( Daughters::const_iterator it = daughters.begin(), end = daughters.end(); it != end; ++it ) {
          decayTree( **it, particles );
        }
      }
    }

    static void getIDVector( const LHCb::Particle& p, bool includeCompositePIDs, std::vector<unsigned int>& ids ) {
      LHCb::Particle::ConstVector particles;
      decayTree( p, particles );
      ids.clear();
      ids.reserve( 80 );

      for ( LHCb::Particle::ConstVector::const_iterator it = particles.begin(), end = particles.end(); it != end;
            ++it ) {
        const LHCb::Particle* particle = *it;

        // Add the pid so that both different decay trees and Particles from the same ProtoParticle
        // have different hashes.
        if ( includeCompositePIDs ) ids.push_back( particle->particleID().pid() );
        if ( !particle->isBasicParticle() ) continue;

        const LHCb::ProtoParticle* pp = particle->proto();
        if ( !pp ) {
          throw GaudiException( "Need access to proto particle to compute hash", "Hash<const LHCb::Particle&>",
                                StatusCode::FAILURE );
        }
        const LHCb::Track* track = pp->track();
        if ( !track ) {
          throw GaudiException( "Need access to track to compute hash", "Hash<const LHCb::Particle&>",
                                StatusCode::FAILURE );
        }
        const std::vector<LHCb::LHCbID>& lhcbIDs = track->lhcbIDs();
        // Add lhcbIDs
        std::transform( lhcbIDs.begin(), lhcbIDs.end(), std::back_inserter( ids ),
                        std::mem_fun_ref( &LHCb::LHCbID::lhcbID ) );
      }
    }

    static size_t hashParticle( const LHCb::Particle& p, bool includeCompositePIDs = true ) {
      std::vector<unsigned int> ids;
      getIDVector( p, includeCompositePIDs, ids );
      return _hash( ids );
    }

    static int intHashParticle( const LHCb::Particle& p, bool includeCompositePIDs = true ) {
      return int( hashParticle( p, includeCompositePIDs ) );
    }

    static bool matchParticles( const LHCb::Particle& a, const LHCb::Particle& b, bool includeCompositePIDs = true ) {
      std::vector<unsigned int> ids_a, ids_b;
      getIDVector( a, includeCompositePIDs, ids_a );
      getIDVector( b, includeCompositePIDs, ids_b );
      return ids_a == ids_b;
    }
  };
} // namespace Swimming

namespace GaudiUtils {

  /// Particle Hash function
  template <>
  struct Hash<const LHCb::Particle&> {
    size_t operator()( const LHCb::Particle& p, bool includeCompositePIDs = true ) const {
      std::vector<unsigned int> ids;
      Swimming::Helpers::getIDVector( p, includeCompositePIDs, ids );
      return _hash( ids );
    }
  };

} // namespace GaudiUtils

namespace LHCb {

  static const CLID CLID_TurningPoint = 901;

  /** @class TurningPoint
   *
   *
   *  @author
   *  @date   2011-10-06
   */
  class TurningPoint : public DataObject {
  public:
    /// Standard constructor
    TurningPoint() : m_raw( 0. ), m_tau( 0. ), m_ip( 0. ), m_dec( 0. ) {}

    TurningPoint( const double raw, const double tau, const double ip, const double dec )
        : m_raw( raw ), m_tau( tau ), m_ip( ip ), m_dec( dec ) {}

    TurningPoint( const double raw, const double tau, const double ip, const double dec,
                  const std::map<std::string, bool>& decisions )
        : m_raw( raw ), m_tau( tau ), m_ip( ip ), m_dec( dec ), m_decisions( decisions ) {}

    TurningPoint( const double raw, const double tau, const double ip, const double dec,
                  const std::map<std::string, bool>&                   decisions,
                  const std::map<std::string, std::map<size_t, bool>>& info )
        : m_raw( raw ), m_tau( tau ), m_ip( ip ), m_dec( dec ), m_decisions( decisions ), m_info( info ) {}

    TurningPoint( const double raw, const double tau, const double ip, const double dec,
                  const std::map<std::string, bool>& decisions, const std::map<std::string, double>& extra )
        : m_raw( raw ), m_tau( tau ), m_ip( ip ), m_dec( dec ), m_decisions( decisions ), m_extra( extra ) {}

    TurningPoint( const double raw, const double tau, const double ip, const double dec,
                  const std::map<std::string, bool>&                   decisions,
                  const std::map<std::string, std::map<size_t, bool>>& info,
                  const std::map<std::string, double>&                 extra )
        : m_raw( raw )
        , m_tau( tau )
        , m_ip( ip )
        , m_dec( dec )
        , m_decisions( decisions )
        , m_info( info )
        , m_extra( extra ) {}

    TurningPoint( const TurningPoint& other )
        : DataObject()
        , m_raw( other.m_raw )
        , m_tau( other.m_tau )
        , m_ip( other.m_ip )
        , m_dec( other.m_dec )
        , m_decisions( other.m_decisions )
        , m_info( other.m_info )
        , m_extra( other.m_extra ) {}

    virtual ~TurningPoint() {}

    // ClassID
    const CLID&        clID() const override { return classID(); }
    static const CLID& classID() { return CLID_TurningPoint; }

    double raw() const { return m_raw; }

    double tau() const { return m_tau; }

    double ip() const { return m_ip; }

    double dec() const { return m_dec; }

    std::vector<std::string> extraNames() const {
      std::vector<std::string> names;
#ifndef WIN32
      for ( std::map<std::string, double>::const_iterator it = m_extra.begin(), end = m_extra.end(); it != end; ++it ) {
        names.push_back( it->first );
      }
#endif
      return names;
    }

    double extra( const std::string& name ) const {
      std::map<std::string, double>::const_iterator it = m_extra.find( name );
      if ( it != m_extra.end() ) {
#ifndef WIN32
        return it->second;
#else
        return 0.;
#endif
      } else {
        std::string msg = "Extra turningpoint with name ";
        msg += name;
        msg += " does not exist.";
        throw GaudiException( msg, "TurningPoint", StatusCode::FAILURE );
      }
    }

    std::vector<std::string> decisions() const {
      std::vector<std::string> d;
#ifndef WIN32
      typedef std::map<std::string, bool> map_t;
      for ( map_t::const_iterator it = m_decisions.begin(), end = m_decisions.end(); it != end; ++it ) {
        d.push_back( it->first );
      }
#endif
      return d;
    }

    bool decision( const std::string& name ) const {
      std::map<std::string, bool>::const_iterator it = m_decisions.find( name );
      if ( it != m_decisions.end() ) {
#ifndef WIN32
        return it->second;
#else
        return true;
#endif
      } else {
        std::stringstream msg;
        msg << "Decision with name " << name << " does not exist.";
        throw GaudiException( msg.str(), "TurningPoint", StatusCode::FAILURE );
      }
    }

    std::vector<std::string> infoNames() const {
      std::vector<std::string> names;
#ifndef WIN32
      for ( std::map<std::string, std::map<size_t, bool>>::const_iterator it = m_info.begin(), end = m_info.end();
            it != end; ++it ) {
        names.push_back( it->first );
      }
#endif
      return names;
    }

    const std::map<size_t, bool>& info( const std::string& name ) const {
      std::map<std::string, std::map<size_t, bool>>::const_iterator it = m_info.find( name );
      if ( it != m_info.end() ) {
        return it->second;
      } else {
        std::stringstream msg;
        msg << "DaughterInfo with name " << name << " does not exist.";
        throw GaudiException( msg.str(), "TurningPoint", StatusCode::FAILURE );
      }
    }

    bool participated( const std::string& name, const LHCb::Particle& daughter ) const {
      size_t                         h = Swimming::Helpers::hashParticle( daughter );
      typedef std::map<size_t, bool> map_t;
      const map_t&                   i  = info( name );
      map_t::const_iterator          it = i.find( h );
      if ( it != i.end() ) {
#ifndef WIN32
        return it->second;
#else
        return true;
#endif
      } else {
        std::stringstream msg;
        msg << "No entry present for daughter with hash " << h << " in info with with name " << name << ".";
        throw GaudiException( msg.str(), "TurningPoint", StatusCode::FAILURE );
      }
    }

  private:
    double m_raw;
    double m_tau;
    double m_ip;
    double m_dec;

    std::map<std::string, bool>                   m_decisions;
    std::map<std::string, std::map<size_t, bool>> m_info;
    std::map<std::string, double>                 m_extra;
  };
} // namespace LHCb
#endif // EVENT_TURNINGPOINT_H
