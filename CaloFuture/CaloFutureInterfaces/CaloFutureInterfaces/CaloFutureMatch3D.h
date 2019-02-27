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
#ifndef CaloFutureMatch3D_h
#define CaloFutureMatch3D_h

#include <Math/SVector.h>
#include <Math/SMatrix.h>

class CaloFutureMatch3D
{
 public:
  using Vector = ROOT::Math::SVector<double, 3>;
  using Matrix = ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> >;

  /// constructor
  CaloFutureMatch3D( const Vector& params, const Matrix& matrix );
  CaloFutureMatch3D()=default;


  const Vector& params() const;
  const Matrix& matrix() const;
  bool ok() const;
  bool inverted() const;

  // invert the matrix
  bool invert();

 private:
  // the vector of parameters x,y(,e)
  Vector m_params;
  // the (inverse) covariance matrix of parameters
  Matrix m_matrix;
  // flag for errors
  bool m_ok = true;
  // flag to indicate that matrix is already inverted
  bool m_inverted = false;
 };

inline CaloFutureMatch3D::CaloFutureMatch3D( const Vector& params, const Matrix& matrix ) : m_params( params ), m_matrix( matrix ), m_ok( true ), m_inverted( false )
{
}

inline const CaloFutureMatch3D::Vector& CaloFutureMatch3D::params() const 
{
 return m_params;
}

inline const CaloFutureMatch3D::Matrix& CaloFutureMatch3D::matrix() const
{ 
 return m_matrix;
}

inline bool CaloFutureMatch3D::ok() const
{ 
 return m_ok;
}

inline bool CaloFutureMatch3D::inverted() const 
{ 
 return m_inverted;
}

inline bool CaloFutureMatch3D::invert()
{ 
 if ( !m_inverted )
  m_inverted = m_ok = m_matrix.Invert();
 return m_ok;
}

#endif
