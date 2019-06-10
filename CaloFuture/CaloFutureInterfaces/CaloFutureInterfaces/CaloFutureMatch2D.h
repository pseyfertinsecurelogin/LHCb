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

namespace Calo::Future {

  class Match2D {
  public:
    using Vector = ROOT::Math::SVector<double, 2>;
    using Matrix = ROOT::Math::SMatrix<double, 2, 2, ROOT::Math::MatRepSym<double, 2>>;

    /// constructor
    Match2D() = default;

    Match2D( const Vector& params, const Matrix& matrix )
        : m_params( params ), m_matrix( matrix ), m_ok( m_matrix.Invert() ) {}

    const Vector& params() const { return m_params; }
    const Matrix& matrix() const { return m_matrix; }
    explicit      operator bool() const { return m_ok; }

  private:
    Vector m_params;     // the vector of parameters x,y(,e)
    Matrix m_matrix;     // the inverse covariance matrix of parameters
    bool   m_ok = false; // flag for errors
  };

} // namespace Calo::Future

#endif
