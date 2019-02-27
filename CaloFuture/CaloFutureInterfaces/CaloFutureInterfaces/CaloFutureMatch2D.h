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

#include <Math/SVector.h>
#include <Math/SMatrix.h>

class CaloFutureMatch2D
{
 public:
  using Vector = ROOT::Math::SVector<double, 2>;
  using Matrix = ROOT::Math::SMatrix<double, 2, 2, ROOT::Math::MatRepSym<double, 2> >;

  /// constructor
  inline CaloFutureMatch2D( const Vector& params, const Matrix& matrix );
  inline CaloFutureMatch2D();
  inline CaloFutureMatch2D( const CaloFutureMatch2D& m );

  inline CaloFutureMatch2D& operator=( const CaloFutureMatch2D& m );

  inline const Vector& params() const;
  inline const Matrix& matrix() const;
  inline bool ok() const;
  inline bool inverted() const;
  inline double& operator()( const unsigned int ind );
  inline double& operator()( const unsigned int ind1, const unsigned int ind2 );

  inline CaloFutureMatch2D& set( const Matrix& m );
  inline CaloFutureMatch2D& set( const Vector& v );
  inline CaloFutureMatch2D& setOK( const bool e );
  inline CaloFutureMatch2D& setInverted( const bool i);

  // invert the matrix
  inline bool invert();

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

CaloFutureMatch2D::CaloFutureMatch2D( const Vector& params, const Matrix& matrix ) : m_params( params ), m_matrix( matrix ), m_ok( true ), m_inverted( false )
{
}

CaloFutureMatch2D::CaloFutureMatch2D() : m_params(), m_matrix(), m_ok( true ), m_inverted( false )
{
}

CaloFutureMatch2D::CaloFutureMatch2D( const CaloFutureMatch2D& m )
{
 m_matrix =  m.m_matrix;
 m_params = m.m_params;
 m_ok =  m.m_ok;
 m_inverted =  m.m_inverted;
}

CaloFutureMatch2D& CaloFutureMatch2D::operator=( const CaloFutureMatch2D& m )
{
 m_matrix =  m.m_matrix;
 m_params = m.m_params;
 m_ok =  m.m_ok;
 m_inverted =  m.m_inverted;
 return *this;
}

const CaloFutureMatch2D::Vector& CaloFutureMatch2D::params() const
{ 
 return m_params;
}

const CaloFutureMatch2D::Matrix& CaloFutureMatch2D::matrix() const
{ 
 return m_matrix;
}

bool CaloFutureMatch2D::ok() const
{ 
 return m_ok;
}

bool CaloFutureMatch2D::inverted() const 
{ 
 return m_inverted;
}

double& CaloFutureMatch2D::operator()( const unsigned int ind )
{ 
 return m_params( ind );
}

double& CaloFutureMatch2D::operator()( const unsigned int ind1, const unsigned int ind2 ) 
{ 
 return m_matrix( ind1, ind2 );
}

CaloFutureMatch2D& CaloFutureMatch2D::set( const CaloFutureMatch2D::Matrix& m )
{
 m_matrix = m;
 return *this;
}

CaloFutureMatch2D& CaloFutureMatch2D::set( const CaloFutureMatch2D::Vector& v )
{
 m_params = v;
 return *this;
}

CaloFutureMatch2D& CaloFutureMatch2D::setOK( const bool e )
{
 m_ok = e;
 return *this;
}

CaloFutureMatch2D& CaloFutureMatch2D::setInverted( const bool i )
{
 m_inverted = i;
 return *this;
}

  // invert the matrix
bool CaloFutureMatch2D::invert()
{ 
 if ( !m_inverted )
  m_inverted = m_ok = m_matrix.Invert();
 return m_ok;
}

#endif

