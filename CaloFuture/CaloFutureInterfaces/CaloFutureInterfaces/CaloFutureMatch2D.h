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
#ifndef CaloFutureMatch2D_h
#define CaloFutureMatch2D_h

#include <Math/SMatrix.h>
#include <Math/SVector.h>
#include <optional>

namespace LHCb::Calo {

  class Match2D {
  public:
    using Vector = ROOT::Math::SVector<double, 2>;
    using Matrix = ROOT::Math::SMatrix<double, 2, 2, ROOT::Math::MatRepSym<double, 2>>;

    /// constructor
    Match2D() = default;
    Match2D( Vector params, Matrix matrix )
        : m_params( std::move( params ) ), m_matrix( std::move( matrix ) ), m_ok( m_matrix.Invert() ) {}

    const Vector& params() const { return m_params; }
    const Matrix& matrix() const { return m_matrix; }
    explicit      operator bool() const { return m_ok; }

    friend std::optional<double> chi2( const Match2D& m1, const Match2D& m2 ) {
      if ( !m1 || !m2 ) return std::nullopt;

      // evaluate the overall covariance matrix
      Matrix cov = m1.matrix() + m2.matrix();
      if ( !cov.Invert() ) return std::nullopt;

      // get the weighted and mean parameters
      // Note: Can NOT use `auto` here
      Vector pw = m1.matrix() * m1.params() + m2.matrix() * m2.params();
      Vector pm = cov * pw;

      // evaluate chi2
      return ROOT::Math::Similarity( pm - m1.params(), m1.matrix() ) +
             ROOT::Math::Similarity( pm - m2.params(), m2.matrix() );
    }

  private:
    Vector m_params;     // the vector of parameters x,y(,e)
    Matrix m_matrix;     // the inverse covariance matrix of parameters
    bool   m_ok = false; // flag for errors
  };

} // namespace LHCb::Calo

#endif
