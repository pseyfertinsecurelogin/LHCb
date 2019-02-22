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
#ifndef PRINTMCDECAYTREETOOL_H
#define PRINTMCDECAYTREETOOL_H 1

// Include files
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// from LHCb
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

// Forward declarations
namespace LHCb {
  class IParticlePropertySvc;
}

class MsgStream;

/** @class PrintMCDecayTreeTool PrintMCDecayTreeTool.h
 *  This is an implementation of the decay tree printing tool,
 *  defined in interface IPrintMCDecayTreeTool
 *
 *  @author Olivier Dormond
 *  @date   29/03/2001
 */
class PrintMCDecayTreeTool : public extends<GaudiTool, IPrintMCDecayTreeTool> {
public:
  /// Standard Constructor
  using base_class::base_class;

  StatusCode initialize() override;

  void printTree( const LHCb::MCParticle* mother, int maxDepth = -1 ) const override;

  /// Print all the MC particles leading to this one.
  void printAncestor( const LHCb::MCParticle* child ) const override;

  void printAsTree( const LHCb::MCParticle::ConstVector& event ) const override;

  void printAsTree( const LHCb::MCParticles& event ) const override;

  void printAsList( const LHCb::MCParticle::ConstVector& event ) const override;

  void printAsList( const LHCb::MCParticles& event ) const override;

private:
  enum InfoKeys {
    Name,
    PID,
    E,
    M,
    P,
    Pt,
    Px,
    Py,
    Pz,
    Vx,
    Vy,
    Vz,
    theta,
    phi,
    eta,
    flags,
    fromSignal,
    idcl,
  };

  MsgStream& printHeader( MsgStream& log ) const;

  MsgStream& printInfo( const std::string& prefix, const LHCb::MCParticle* part, MsgStream& log ) const;
  MsgStream& printDecayTree( const LHCb::MCParticle* mother, const std::string& prefix, int depth,
                             MsgStream& log ) const;

  SmartIF<LHCb::IParticlePropertySvc> m_ppSvc;                                 ///< Pointer to particle property service
  Gaudi::Property<int>                m_depth{this, "PrintDepth", 999};        ///< Depth of printing for tree
  Gaudi::Property<int>                m_treeWidth{this, "TreeWidth", 20};      ///< width of the tree drawing
  Gaudi::Property<int>                m_fWidth{this, "FieldWidth", 10};        ///< width of the data fields
  Gaudi::Property<int>                m_fPrecision{this, "FieldPrecision", 2}; ///< precision of the data fields
  Gaudi::Property<std::string>        m_arrow{this, "Arrow", "+-->"};          ///< arrow drawing
  Gaudi::Property<std::string> m_informationsDeprecated{this, "Informations"}; ///< For backward compatibility. Will be
                                                                               ///< removed
  Gaudi::Property<std::string> m_information{this, "Information",
                                             "Name E M P Pt phi Vz"}; ///< The specification of the values to print
  std::vector<InfoKeys>        m_keys;                                ///< The list of information to print
  Gaudi::Property<double> m_energyUnit{this, "EnergyUnit", Gaudi::Units::MeV}; /// Unit for energies, momenta and masses
  Gaudi::Property<double> m_lengthUnit{this, "LengthUnit", Gaudi::Units::mm};  /// Unit for distances
  std::string             m_energyUnitName{"MeV"}; ///< Unit for energies, momenta and masses
  std::string             m_lengthUnitName{"mm"};  ///< Unit for distances
};
#endif // PRINTMDECAYTREETOOL_H
