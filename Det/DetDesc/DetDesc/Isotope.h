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
#ifndef DETDESC_ISOTOPE_H
#define DETDESC_ISOTOPE_H

// Include files
#include "DetDesc/Material.h"

#include "DetDesc/CLIDIsotope.h"

/** @class Isotope Isotope.h DetDesc/Isotope.h

    Isotope class defines the Isotope material component.
    An Element material component can defined by composition of Isotopes.
    See the Element class for details.

   @author Radovan Chytracek
*/
class Isotope final : public Material {

public:
  /// Constructors
  Isotope( const std::string name = "", const double a = 0, const double z = 0, const double i = 0,
           const double density = 0, const double rl = 0, const double al = 0,
           const double temp = Gaudi::Units::STP_Temperature, const double press = Gaudi::Units::STP_Pressure,
           const eState s = stateUndefined );

  //        Atomic mass [g/mole]
  inline double A() const override;
  inline void   setA( const double value ) override;

  //        Atomic number
  inline double Z() const override;
  inline void   setZ( const double value ) override;

  //        Mean excitation energy
  inline double I() const override;
  inline void   setI( const double value ) override;

  //       Parameters for density effect correction
  inline double C() const override;
  inline void   setC( const double value ) override;
  inline double a() const override;
  inline void   seta( const double value ) override;
  inline double m() const override;
  inline void   setm( const double value ) override;
  inline double X0() const override;
  inline void   setX0( const double value ) override;
  inline double X1() const override;
  inline void   setX1( const double value ) override;

  //        Number of nucleons
  inline double N() const override;

  inline const CLID& clID() const override { return Isotope::classID(); }
  static const CLID& classID() { return CLID_Isotope; }

protected:
  //        Atomic mass [g/mole]
  double m_A;

  //        Atomic number
  double m_Z;

  //        Excitation energy
  double m_I;

  //        Parameters for density effect correction
  double m_C;
  double m_a;
  double m_m;
  double m_X0;
  double m_X1;
};

///////////////////////////////////////////////////////////////////////////
inline double Isotope::A() const { return m_A; }
inline void   Isotope::setA( const double value ) { m_A = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::Z() const { return m_Z; }
inline void   Isotope::setZ( const double value ) { m_Z = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::I() const { return m_I; }
inline void   Isotope::setI( const double value ) { m_I = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::C() const { return m_C; }
inline void   Isotope::setC( const double value ) { m_C = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::a() const { return m_a; }
inline void   Isotope::seta( const double value ) { m_a = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::m() const { return m_m; }
inline void   Isotope::setm( const double value ) { m_m = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::X0() const { return m_X0; }
inline void   Isotope::setX0( const double value ) { m_X0 = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::X1() const { return m_X1; }
inline void   Isotope::setX1( const double value ) { m_X1 = value; }
///////////////////////////////////////////////////////////////////////////
inline double Isotope::N() const { return m_A * Gaudi::Units::mole / Gaudi::Units::g; }
///////////////////////////////////////////////////////////////////////////

#endif // DETDESC_ISOTOPE_H
