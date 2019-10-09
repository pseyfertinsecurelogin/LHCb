/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/VectorMap.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_VertexBase = 811;

  /** @class VertexBase VertexBase.h
   *
   * Base class from which all vertices are derived
   *
   * @author Juan Palacios
   *
   */

  class VertexBase : public KeyedObject<int> {
  public:
    /// typedef for std::vector of VertexBase
    typedef std::vector<VertexBase*>       Vector;
    typedef std::vector<const VertexBase*> ConstVector;

    /// typedef for KeyedContainer of VertexBase
    typedef KeyedContainer<VertexBase, Containers::HashMap> Container;

    /// User information
    typedef GaudiUtils::VectorMap<int, double> ExtraInfo;
    /// For uniform access to containers in TES (KeyedContainer,SharedContainer)
    typedef Gaudi::NamedRange_<ConstVector> Range;

    /// Describe how the vertex was made
    enum VertexType { Primary = 100 };

    /// Copy constructor
    VertexBase( const VertexBase& rhs )
        : KeyedObject<int>()
        , m_position( rhs.m_position )
        , m_covMatrix( rhs.m_covMatrix )
        , m_chi2( rhs.m_chi2 )
        , m_nDoF( rhs.m_nDoF )
        , m_extraInfo( rhs.m_extraInfo ) {}

    /// create a vertex with a selected key
    explicit VertexBase( int key )
        : KeyedObject<int>( key )
        , m_position( 0.0, 0.0, -100 * Gaudi::Units::m )
        , m_covMatrix()
        , m_chi2( -1.0 )
        , m_nDoF( -1 )
        , m_extraInfo() {}

    /// Constructor from a point
    VertexBase( const Gaudi::XYZPoint& point )
        : KeyedObject<int>(), m_position( point ), m_covMatrix(), m_chi2( -1.0 ), m_nDoF( -1 ), m_extraInfo() {}

    /// Default Constructor
    VertexBase()
        : m_position( 0.0, 0.0, -100 * Gaudi::Units::m ), m_covMatrix(), m_chi2( -1.0 ), m_nDoF( -1 ), m_extraInfo() {}

    /// Default Destructor
    virtual ~VertexBase() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone vertex
    virtual VertexBase* clone() const;

    /// Assignment operator
    VertexBase& operator=( const VertexBase& rhs );

    /// Retrieve the Chi^2/DoF of vertex
    double chi2PerDoF() const;

    /// Set the Chi^2 and the DoF of the vertex (fit)
    void setChi2AndDoF( double chi2, int ndof );

    /// ExtraInformation. Don't use directly, use hasInfo, info, addInfo...
    const ExtraInfo& extraInfo() const;

    /// has information for specified key
    bool hasInfo( int key ) const;

    ///  Add new information associated with the specified key. This method cannot be used to modify information for a
    ///  pre-existing key.
    bool addInfo( const int key, const double info );

    /// extract the information associated with the given key. If there is no such infomration the default value will be
    /// returned.
    double info( const int key, const double def ) const;

    /// erase the information associated with the given key
    unsigned long eraseInfo( int key );

    /// Is the vertex a primary?
    virtual bool isPrimary() const;

    /// Print this Particle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Position in LHCb reference system
    const Gaudi::XYZPoint& position() const;

    /// Update  Position in LHCb reference system
    void setPosition( const Gaudi::XYZPoint& value );

    /// Retrieve const  Covariance matrix containing errors on vertex position
    const Gaudi::SymMatrix3x3& covMatrix() const;

    /// Update  Covariance matrix containing errors on vertex position
    void setCovMatrix( const Gaudi::SymMatrix3x3& value );

    /// Retrieve const  Chi square of vertex fit
    double chi2() const;

    /// Update  Chi square of vertex fit
    void setChi2( double value );

    /// Retrieve const  Number of degree of freedom
    int nDoF() const;

    /// Update  Number of degree of freedom
    void setNDoF( int value );

    /// Update  Some addtional user information. Don't use directly. Use *Info() methods.
    void setExtraInfo( const ExtraInfo& value );

    friend std::ostream& operator<<( std::ostream& str, const VertexBase& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    Gaudi::XYZPoint     m_position;  ///< Position in LHCb reference system
    Gaudi::SymMatrix3x3 m_covMatrix; ///< Covariance matrix containing errors on vertex position
    double              m_chi2;      ///< Chi square of vertex fit
    int                 m_nDoF;      ///< Number of degree of freedom
    ExtraInfo           m_extraInfo; ///< Some addtional user information. Don't use directly. Use *Info() methods.

  }; // class VertexBase

  /// Definition of Keyed Container for VertexBase
  typedef KeyedContainer<VertexBase, Containers::HashMap> VertexBases;

  inline std::ostream& operator<<( std::ostream& s, LHCb::VertexBase::VertexType e ) {
    switch ( e ) {
    case LHCb::VertexBase::Primary:
      return s << "Primary";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VertexBase::VertexType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VertexBase::clID() const { return LHCb::VertexBase::classID(); }

inline const CLID& LHCb::VertexBase::classID() { return CLID_VertexBase; }

inline const Gaudi::XYZPoint& LHCb::VertexBase::position() const { return m_position; }

inline void LHCb::VertexBase::setPosition( const Gaudi::XYZPoint& value ) { m_position = value; }

inline const Gaudi::SymMatrix3x3& LHCb::VertexBase::covMatrix() const { return m_covMatrix; }

inline void LHCb::VertexBase::setCovMatrix( const Gaudi::SymMatrix3x3& value ) { m_covMatrix = value; }

inline double LHCb::VertexBase::chi2() const { return m_chi2; }

inline void LHCb::VertexBase::setChi2( double value ) { m_chi2 = value; }

inline int LHCb::VertexBase::nDoF() const { return m_nDoF; }

inline void LHCb::VertexBase::setNDoF( int value ) { m_nDoF = value; }

inline void LHCb::VertexBase::setExtraInfo( const ExtraInfo& value ) { m_extraInfo = value; }

inline LHCb::VertexBase& LHCb::VertexBase::operator=( const VertexBase& rhs ) {

  if ( this != &rhs ) {
    m_position  = rhs.m_position;
    m_covMatrix = rhs.m_covMatrix;
    m_chi2      = rhs.m_chi2;
    m_nDoF      = rhs.m_nDoF;
    m_extraInfo = rhs.m_extraInfo;
  }
  return *this;
}

inline double LHCb::VertexBase::chi2PerDoF() const { return ( ( m_nDoF > 0 ) ? m_chi2 / double( m_nDoF ) : -1. ); }

inline void LHCb::VertexBase::setChi2AndDoF( double chi2, int ndof ) {

  m_chi2 = chi2;
  m_nDoF = ndof;
}

inline const LHCb::VertexBase::ExtraInfo& LHCb::VertexBase::extraInfo() const { return m_extraInfo; }

inline bool LHCb::VertexBase::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::VertexBase::addInfo( const int key, const double info ) {

  return m_extraInfo.insert( key, info ).second;
}

inline double LHCb::VertexBase::info( const int key, const double def ) const {

  ExtraInfo::iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline unsigned long LHCb::VertexBase::eraseInfo( int key ) { return m_extraInfo.erase( key ); }
