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
#ifndef IFUTUREGAMMAPI0SEPARATIONTOOL_H
#define IFUTUREGAMMAPI0SEPARATIONTOOL_H 1

// Include files
// from STL
#include <map>
#include <optional>

// from Gaudi
#include "CaloFutureInterfaces/Enums.h"
#include "Event/CaloHypo.h"
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/STLExtensions.h"

/** @class IFutureGammaPi0SeparationTool IFutureGammaPi0SeparationTool.h
 *
 *
 *  @author Miriam Calvo Gomez --
 *  @date   2019-03-28
 */
namespace LHCb::Calo::Interfaces {
  struct IGammaPi0Separation : extend_interfaces<IAlgTool> {

    struct Observables {
      double fr2{0};
      double fasym{0};
      double fkappa{0};
      double fr2r4{0};
      double Eseed{0};
      double E2{0};
      double Ecl{0};
      int    area{0};
    };
    // Return the interface ID
    DeclareInterfaceID( IGammaPi0Separation, 2, 0 );

    virtual std::optional<double>      isPhoton( const CaloHypo& hypo ) const           = 0;
    virtual std::optional<double>      isPhoton( span<const double> ) const             = 0;
    virtual std::optional<double>      isPhoton( Observables const& observables ) const = 0;
    virtual std::optional<Observables> observables( const CaloHypo& hypo ) const        = 0;
  };
} // namespace LHCb::Calo::Interfaces
#endif // IFUTUREGAMMAPI0SEPARATIONTOOL_H
