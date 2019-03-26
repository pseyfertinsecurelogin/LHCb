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
/// DetDesc
#include "DetDesc/Isotope.h"
///

////////////////////////////////////////////////////////////////////////////////////
Isotope::Isotope( const std::string name, const double A, const double z, const double i, const double density,
                  const double rl, const double al, const double temp, const double press, const eState s )
    : Material( name, density, rl, al, temp, press, s )
    , m_A( A )
    , m_Z( z )
    , m_I( i )
    , m_C( 0 )
    , m_a( 0. )
    , m_m( 0. )
    , m_X0( 0. )
    , m_X1( 0. ) {}
////////////////////////////////////////////////////////////////////////////////////
