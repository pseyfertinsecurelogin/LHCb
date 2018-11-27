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
// Include files

// from Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/PhysicalConstants.h"

// From PartProp
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

// from Event
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

// local
#include "PrintMCDecayTreeTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintMCDecayTreeTool
//
// 29/03/2001 : Olivier Dormond
//-----------------------------------------------------------------------------

// Declaration of the AlgTool Factory

DECLARE_COMPONENT( PrintMCDecayTreeTool )

using namespace Gaudi::Units;

//=============================================================================
// initialise
//=============================================================================
StatusCode PrintMCDecayTreeTool::initialize( ){

  StatusCode sc = base_class::initialize();
  if (!sc) return sc;

  m_ppSvc = service( "LHCb::ParticlePropertySvc",true);

  if ( m_informationsDeprecated != "" ){
    warning() << "You are using the deprecated option ``informations''." << endmsg ;
    warning() << "Use ``Information'' instead." << endmsg ;
    m_information = m_informationsDeprecated ;
  }


  std::size_t oldpos = 0, pos;
  do {
    pos=m_information.value().find( ' ', oldpos );
    std::string tok{m_information.value(), oldpos, pos-oldpos};
    if( tok=="Name" )       m_keys.push_back(Name);
    else if( tok=="PID" )   m_keys.push_back(PID);
    else if( tok=="E" )     m_keys.push_back(E);
    else if( tok=="M" )     m_keys.push_back(M);
    else if( tok=="P" )     m_keys.push_back(P);
    else if( tok=="Pt" )    m_keys.push_back(Pt);
    else if( tok=="Px" )    m_keys.push_back(Px);
    else if( tok=="Py" )    m_keys.push_back(Py);
    else if( tok=="Pz" )    m_keys.push_back(Pz);
    else if( tok=="Vx" )    m_keys.push_back(Vx);
    else if( tok=="Vy" )    m_keys.push_back(Vy);
    else if( tok=="Vz" )    m_keys.push_back(Vz);
    else if( tok=="theta" ) m_keys.push_back(theta);
    else if( tok=="phi" )   m_keys.push_back(phi);
    else if( tok=="eta" )   m_keys.push_back(eta);
    else if( tok=="flags" ) m_keys.push_back(flags);
    else if( tok=="fromSignal") m_keys.push_back(fromSignal);
    else if( tok=="IDCL" )  m_keys.push_back(idcl);
    else
      err() << "Unknown output key '" << tok << "'. Ignoring it."
            << endmsg;
    if( pos != std::string::npos ) oldpos = pos+1;
    else                           oldpos = pos;
  }
  while( pos != std::string::npos );

  if (m_energyUnit == TeV) m_energyUnitName = "TeV" ;
  else if (m_energyUnit == GeV) m_energyUnitName = "GeV" ;
  else if (m_energyUnit == MeV) m_energyUnitName = "MeV" ;
  else if (m_energyUnit <= 0) {
    err() << "You have chosen a unit for energies: "
          << m_energyUnit << endmsg;
    return StatusCode::FAILURE ;
  }
  else {
    warning() << "You have chosen a non-standard unit for energies: "
              << m_energyUnit << endmsg;
    m_energyUnitName = "???" ;
  }
  if (m_lengthUnit == mm) m_lengthUnitName = "mm" ;
  else if (m_lengthUnit == cm) m_lengthUnitName = "cm" ;
  else if (m_lengthUnit == m) m_lengthUnitName = "m" ;
  else if (m_lengthUnit <= 0) {
    err() << "You have chosen a unit for lengths: "
          << m_lengthUnit << endmsg;
    return StatusCode::FAILURE ;
  }
  else {
    warning() << "You have chosen a non-standard unit for lengths: "
              << m_lengthUnit << endmsg;
    m_lengthUnitName = "??" ;

  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// printHeader
//=============================================================================
MsgStream& PrintMCDecayTreeTool::printHeader( MsgStream& log ) const
{
  static const std::string mctitle = " MCParticle ";
  const std::string* title;
  title = &mctitle;

  bool name_key = ( std::find(m_keys.begin(), m_keys.end(), Name)!=m_keys.end() );

  int n_keys = m_keys.size() - (name_key ? 1 : 0);
  int width = n_keys*m_fWidth + (name_key ? m_treeWidth.value() : 0);
  int left  = (width - title->length() - 2 )/2;
  int right =  width - title->length() - 2 - left;

  if( left >= 0 )
    log << '<' << std::string(left,'-');
  log << *title;
  if( right >= 0 )
    log<< std::string(right,'-') << '>';

  log << std::endl;

  for( const auto& i : m_keys)
    switch( i ) {
    case Name:      log << std::setw(m_treeWidth) << "Name";   break;
    case PID:       log << std::setw(m_fWidth) << "PID";         break;
    case E:         log << std::setw(m_fWidth) << "E";         break;
    case M:         log << std::setw(m_fWidth) << "M";         break;
    case P:         log << std::setw(m_fWidth) << "P";         break;
    case Pt:        log << std::setw(m_fWidth) << "Pt";        break;
    case Px:        log << std::setw(m_fWidth) << "Px";        break;
    case Py:        log << std::setw(m_fWidth) << "Py";        break;
    case Pz:        log << std::setw(m_fWidth) << "Pz";        break;
    case Vx:        log << std::setw(m_fWidth) << "Vx";        break;
    case Vy:        log << std::setw(m_fWidth) << "Vy";        break;
    case Vz:        log << std::setw(m_fWidth) << "Vz";        break;
    case theta:     log << std::setw(m_fWidth) << "theta";     break;
    case phi:       log << std::setw(m_fWidth) << "phi";       break;
    case eta:       log << std::setw(m_fWidth) << "eta";       break;
    case flags:     log << std::setw(m_fWidth) << "Flags ";    break;
    case fromSignal:  log <<  std::setw(m_fWidth) << "fromSignal";     break;
    case idcl:      log << std::setw(m_fWidth) << "ID CL";     break;
    }

  log << std::endl;

  for( const auto& i : m_keys)
    switch( i ) {
    case Name:      log << std::setw(m_treeWidth) << " ";      break;
    case PID:       log << std::setw(m_fWidth)    << " ";       break;
    case E:         log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case M:         log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case P:         log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case Pt:        log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case Px:        log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case Py:        log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case Pz:        log << std::setw(m_fWidth) << m_energyUnitName;       break;
    case Vx:        log << std::setw(m_fWidth) << m_lengthUnitName;        break;
    case Vy:        log << std::setw(m_fWidth) << m_lengthUnitName;        break;
    case Vz:        log << std::setw(m_fWidth) << m_lengthUnitName;        break;
    case theta:     log << std::setw(m_fWidth) << "mrad";      break;
    case phi:       log << std::setw(m_fWidth) << "mrad";      break;
    case eta:       log << std::setw(m_fWidth) << "prap";      break;
    case flags:     log << std::setw(m_fWidth) << " ";     break;
    case fromSignal:  log <<  std::setw(m_fWidth) << " ";     break;
    case idcl:      log << std::setw(m_fWidth) << " ";         break;
    }

  return log << std::endl;
}
//=============================================================================
// printInfo (MCParticle)
//=============================================================================
MsgStream& PrintMCDecayTreeTool::printInfo( const std::string &prefix,
                                            const LHCb::MCParticle *part,
                                            MsgStream &log ) const
{
  const LHCb::ParticleProperty* p = m_ppSvc->find( part->particleID() );
  const LHCb::MCVertex *origin = part->originVertex();

  for(const auto& i : m_keys)
    switch( i ) {
    case Name:
      {
        std::string p_name = p ? p->particle() : "N/A";
        int p_len = p_name.length();
        if( prefix.length() == 0 ) {
          if( p_len > m_treeWidth )
            p_len = m_treeWidth - 1;
          log << p_name << std::string( m_treeWidth - p_len - 1,' ' );
        } else {
          if( p_len > (int)(m_treeWidth-prefix.length()-m_arrow.length()) )
            p_len = m_treeWidth - prefix.length() - m_arrow.length() - 1;
          log << prefix.substr(0, prefix.length()-1) << m_arrow << p_name
              << std::string( m_treeWidth - prefix.length()
                              - m_arrow.length() - p_len, ' ' );
        }
      }
      break;
    case PID:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->particleID().pid() ;
      break;
    case E:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().e()/m_energyUnit;
      break;
    case M:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().M()/m_energyUnit;
      break;
    case P:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().Vect().R()/m_energyUnit;
      break;
    case Pt:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().Pt()/m_energyUnit;
      break;
    case Px:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().px()/m_energyUnit;
      break;
    case Py:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().py()/m_energyUnit;
      break;
    case Pz:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().pz()/m_energyUnit;
      break;
    case Vx:
      if( origin )
        log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
            << origin->position().x()/m_lengthUnit;
      else
        log << std::setw(m_fWidth) << " N/A ";
      break;
    case Vy:
      if( origin )
        log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
            << origin->position().y()/m_lengthUnit;
      else
        log << std::setw(m_fWidth) << " N/A ";
      break;
    case Vz:
      if( origin )
        log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
            << origin->position().z()/m_lengthUnit;
      else
        log << std::setw(m_fWidth) << " N/A ";
      break;
    case theta:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().theta()/mrad;
      break;
    case phi:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().phi()/mrad;
      break;
    case eta:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->momentum().Eta();
      break;
    case flags:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->flags();
      break;
    case fromSignal:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << part->fromSignal();
      break;
    case idcl:
      log << std::setw(m_fWidth) << std::setprecision(m_fPrecision)
          << 1.0;
      break;
    default:
      break;
    }

  return log << std::endl;
}
//=============================================================================
// Print decay tree for a given MCparticle
//=============================================================================
void PrintMCDecayTreeTool::printTree( const LHCb::MCParticle* mother,
                                      int maxDepth ) const
{
  if( maxDepth == -1 ) maxDepth = m_depth;

  if( !mother ) {
    err() << "printTree called with NULL MCParticle" << endmsg;
    return;
  }

  auto& log = printHeader( info() << '\n' );
  log.setf(std::ios::fixed,std::ios::floatfield);
  printDecayTree( mother, "", maxDepth, log ) << endmsg;
}
//=============================================================================
// printAncestor (MCParticle)
//=============================================================================
void PrintMCDecayTreeTool::printAncestor( const LHCb::MCParticle *child ) const
{
  const LHCb::ParticleProperty *p = m_ppSvc->find(child->particleID());
  std::string decay = p ? p->particle() : "N/A";
  const LHCb::MCVertex *origin = child->originVertex();
  while( origin && (child = origin->mother()) ) {
    p = m_ppSvc->find(child->particleID());
    decay = (p ? p->particle() : std::string("N/A")) + " -> "+ decay;
    origin = child->originVertex();
  }
  info() << decay << endmsg;
}
//=============================================================================
// printAsTree (MCParticle)
//=============================================================================
void PrintMCDecayTreeTool::printAsTree( const LHCb::MCParticle::ConstVector &event) const
{
  auto& log = printHeader( info() << '\n' );
  log.setf(std::ios::fixed,std::ios::floatfield);
  for(const auto& i : event) {
    if( !i->originVertex() || !i->originVertex()->mother()  )
      printDecayTree( i, "", m_depth, log );
  }
  log << endmsg;
}
//=============================================================================
// printAsTree (MCParticle) (KeyedContainer)
//=============================================================================
void PrintMCDecayTreeTool::printAsTree( const LHCb::MCParticles& event ) const
{
  auto& log = printHeader( info() << '\n' );
  log.setf(std::ios::fixed,std::ios::floatfield);
  for(const auto& i : event) {
    if( !i->originVertex() || !i->originVertex()->mother() )
      printDecayTree( i, "", m_depth, log );
  }
  log << endmsg;
}
//=============================================================================
// printDecayTree (MCParticle)
//=============================================================================
MsgStream& PrintMCDecayTreeTool::printDecayTree( const LHCb::MCParticle *mother,
                                           const std::string &prefix,
                                           int depth,
                                           MsgStream &log ) const
{
  printInfo( prefix, mother, log );
  if( depth ) {
    for ( auto iv = mother->endVertices().begin();
          iv != mother->endVertices().end(); iv++ ) {
      for ( auto idau = (*iv)->products().begin();
            idau != (*iv)->products().end(); idau++ ) {
        bool last = ( (*idau == (*iv)->products().back())
             && (*iv == mother->endVertices().back()) );
        printDecayTree( *idau, prefix+(last?' ':'|'), depth-1, log );
      }
    }
  }
  return log;
}
//=============================================================================
// printAsList (MCParticle)
//=============================================================================
void PrintMCDecayTreeTool::printAsList( const LHCb::MCParticle::ConstVector &event) const
{
  auto& log = printHeader( info() << '\n' );
  for(const auto& i : event) printInfo( "", i, log );
  log << endmsg;
}
//=============================================================================
// printAsList (MCParticle) (KeyedContainer)
//=============================================================================
void PrintMCDecayTreeTool::printAsList( const LHCb::MCParticles &event) const
{
  auto& log = printHeader( info() << '\n' );
  for(const auto& i : event) printInfo( "", i, log );
  log << endmsg;
}
//=============================================================================
//=============================================================================
