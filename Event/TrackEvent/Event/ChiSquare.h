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
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class ChiSquare ChiSquare.h
   *
   * structure holding a chisquare and associated number of dofs
   *
   * @author Wouter Hulsbergen
   *
   */

  class ChiSquare final {
  public:
    /// Constructor
    ChiSquare( const double chi2, int ndof ) : m_chi2( chi2 ), m_nDoF( ndof ) {}

    /// Default Constructor
    ChiSquare() : m_chi2( 0.0 ), m_nDoF( 0 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// return chi2/ndof if ndof>0. returns zero otherwise.
    double chi2PerDoF() const;

    /// return chisquare upper tail probability if ndof>0. returns zero otherwise.
    double prob() const;

    /// addition operator
    ChiSquare& operator+=( const LHCb::ChiSquare& rhs );

    /// subtraction operator
    ChiSquare& operator-=( const LHCb::ChiSquare& rhs );

    /// addition operator
    ChiSquare operator+( const LHCb::ChiSquare& rhs ) const;

    /// subtraction operator
    ChiSquare operator-( const LHCb::ChiSquare& rhs ) const;

    /// Retrieve const  chi square
    double chi2() const;

    /// Retrieve const  number of degrees of freedom
    int nDoF() const;

    friend std::ostream& operator<<( std::ostream& str, const ChiSquare& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double m_chi2; ///< chi square
    int    m_nDoF; ///< number of degrees of freedom

  }; // class ChiSquare

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::ChiSquare::fillStream( std::ostream& s ) const {
  s << "{ "
    << "chi2 :	" << (float)m_chi2 << std::endl
    << "nDoF :	" << m_nDoF << std::endl
    << " }";
  return s;
}

inline double LHCb::ChiSquare::chi2() const { return m_chi2; }

inline int LHCb::ChiSquare::nDoF() const { return m_nDoF; }

inline double LHCb::ChiSquare::chi2PerDoF() const { return m_nDoF > 0 ? m_chi2 / m_nDoF : 0; }

inline LHCb::ChiSquare& LHCb::ChiSquare::operator+=( const LHCb::ChiSquare& rhs ) {

  m_chi2 += rhs.m_chi2;
  m_nDoF += rhs.m_nDoF;
  return *this;
}

inline LHCb::ChiSquare& LHCb::ChiSquare::operator-=( const LHCb::ChiSquare& rhs ) {

  m_chi2 -= rhs.m_chi2;
  m_nDoF -= rhs.m_nDoF;
  return *this;
}

inline LHCb::ChiSquare LHCb::ChiSquare::operator+( const LHCb::ChiSquare& rhs ) const {

  return LHCb::ChiSquare( m_chi2 + rhs.chi2(), m_nDoF + rhs.nDoF() );
}

inline LHCb::ChiSquare LHCb::ChiSquare::operator-( const LHCb::ChiSquare& rhs ) const {

  return LHCb::ChiSquare( m_chi2 - rhs.chi2(), m_nDoF - rhs.nDoF() );
}
