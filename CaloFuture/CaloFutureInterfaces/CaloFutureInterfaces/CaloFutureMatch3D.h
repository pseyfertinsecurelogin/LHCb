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
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "Event/CaloPosition.h"
#include "Event/State.h"
#include "DetDesc/Condition.h"
#include "Kernel/ILHCbMagnetSvc.h"

#ifndef CaloFutureMatch3D_h
#define CaloFutureMatch3D_h

class CaloFutureMatch3D
{
 public:
  using Vector = ROOT::Math::SVector<double, 3>;
  using Matrix = ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> >;

  /// constructor
  inline CaloFutureMatch3D( const Vector& params, const Matrix& matrix );
  inline CaloFutureMatch3D();
  inline CaloFutureMatch3D( const CaloFutureMatch3D& m );

  inline CaloFutureMatch3D& operator=( const CaloFutureMatch3D& m );

  inline const Vector& params() const;
  inline const Matrix& matrix() const;
  inline bool ok() const;
  inline bool inverted() const;

  inline double& operator()( const unsigned int ind );
  inline double& operator()( const unsigned int ind1, const unsigned int ind2 );

  inline CaloFutureMatch3D& set( const Matrix& m );
  inline CaloFutureMatch3D& set( const Vector& v );
  inline CaloFutureMatch3D& setOK( const bool e );
  inline CaloFutureMatch3D& setInverted( const bool i );

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

CaloFutureMatch3D::CaloFutureMatch3D( const Vector& params, const Matrix& matrix ) : m_params( params ), m_matrix( matrix ), m_ok( true ), m_inverted( false )
{
}

CaloFutureMatch3D::CaloFutureMatch3D() : m_params(), m_matrix(), m_ok( true ), m_inverted( false )
{
}

CaloFutureMatch3D::CaloFutureMatch3D( const CaloFutureMatch3D& m )
{
 m_matrix =  m.m_matrix;
 m_params = m.m_params;
 m_ok =  m.m_ok;
 m_inverted =  m.m_inverted;
}

CaloFutureMatch3D& CaloFutureMatch3D::operator=( const CaloFutureMatch3D& m )
{
 m_matrix =  m.m_matrix;
 m_params = m.m_params;
 m_ok =  m.m_ok;
 m_inverted =  m.m_inverted;
 return *this;
}

const CaloFutureMatch3D::Vector& CaloFutureMatch3D::params() const 
{
 return m_params;
}

const CaloFutureMatch3D::Matrix& CaloFutureMatch3D::matrix() const
{ 
 return m_matrix;
}

bool CaloFutureMatch3D::ok() const
{ 
 return m_ok;
}

bool CaloFutureMatch3D::inverted() const 
{ 
 return m_inverted;
}

double& CaloFutureMatch3D::operator()( const unsigned int ind )
{ 
 return m_params( ind );
}

double& CaloFutureMatch3D::operator()( const unsigned int ind1, const unsigned int ind2 )
{ 
 return m_matrix( ind1, ind2 ); 
}

CaloFutureMatch3D& CaloFutureMatch3D::set( const CaloFutureMatch3D::Matrix& m )
{
 m_matrix = m;
 return *this;
}

CaloFutureMatch3D& CaloFutureMatch3D::set( const CaloFutureMatch3D::Vector& v )
{
 m_params = v;
 return *this;
}

CaloFutureMatch3D& CaloFutureMatch3D::setOK( const bool e )
{
 m_ok = e;
 return *this;
}

CaloFutureMatch3D& CaloFutureMatch3D::setInverted( const bool i )
{
 m_inverted = i;
 return *this;
}

bool CaloFutureMatch3D::invert()
{ 
 if ( !m_inverted )
  m_inverted = m_ok = m_matrix.Invert();
 return m_ok;
}

#endif

