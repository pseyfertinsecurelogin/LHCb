#ifndef DAVINCIASSOCIATORS_PARTICLE2MCLINKER_H
#define DAVINCIASSOCIATORS_PARTICLE2MCLINKER_H 1

// Include files
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/compose.h"

#include "boost/variant.hpp"

#include "Linker/LinkedTo.h"
#include "Linker/LinkedFrom.h"
#include "Linker/LinkerWithKey.h"

// DaVinciMCKernel
#include "Kernel/Particle2MCMethod.h"
#include "Kernel/MCAssociation.h"

// Event model
#include "Event/MCParticle.h"
#include "Event/Particle.h"
#include "Event/ProtoParticle.h"

/** @class Object2MCLinker Particle2MCLinker.h Kernel/Particle2MCLinker.h
    *
    *  Class providing association functionality to MCParticles
    *
    *  @author Philippe Charpentier
    *  @date   2004-04-29
    */

template <class SOURCE=LHCb::Particle>
class Object2MCLinker
{
public:
  // Typedef for source type
  typedef SOURCE Source;
  // Typedef for RangeFrom return type
  typedef std::vector<MCAssociation> ToRange;
  typedef ToRange::iterator ToIterator;
  typedef ToRange::const_iterator ToConstIterator;
  // Constructors from Algorithm
  Object2MCLinker( const Algorithm* myMother,
                   const int method,
                   std::vector<std::string> containerList)
    : m_parent(dynamic_cast<const GaudiAlgorithm*>(myMother))
    , m_extension(Particle2MCMethod::extension[method])
    , m_linkerAlgType(Particle2MCMethod::algType[method])
    , m_containerList(std::move(containerList))
    , m_linkTo( myMother->evtSvc(),nullptr,"")
    , m_linkerTable( myMother->evtSvc(),nullptr,"") { }

  Object2MCLinker( const Algorithm* myMother,
                   const int method,
                   const std::string& container)
    : m_parent(dynamic_cast<const GaudiAlgorithm*>(myMother))
    , m_extension(Particle2MCMethod::extension[method])
    , m_linkerAlgType(Particle2MCMethod::algType[method])
    , m_containerList(std::vector<std::string>(1,container))
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const Algorithm* myMother,
                   const std::string& algType,
                   const std::string& extension,
                   std::vector<std::string> containerList)
    : m_parent(dynamic_cast<const GaudiAlgorithm*>(myMother))
    , m_extension(extension)
    , m_linkerAlgType(algType)
    , m_containerList(std::move(containerList))
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const Algorithm* myMother,
                   const std::string& algType,
                   const std::string& extension,
                   const std::string& container)
    : m_parent(dynamic_cast<const GaudiAlgorithm*>(myMother))
    , m_extension(extension)
    , m_linkerAlgType(algType)
    , m_containerList(std::vector<std::string>(1,container))
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const Algorithm* myMother )
    : m_parent(dynamic_cast<const GaudiAlgorithm*>(myMother))
    , m_containerList(std::vector<std::string>())
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  // Now constructors from tools
  Object2MCLinker( const GaudiTool* myMother,
                   const int method,
                   std::vector<std::string> containerList)
    : m_parent(myMother)
    , m_extension(Particle2MCMethod::extension[method])
    , m_linkerAlgType(Particle2MCMethod::algType[method])
    , m_containerList(std::move(containerList))
    , m_linkTo( myMother->evtSvc(),nullptr,"")
    , m_linkerTable( myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const GaudiTool* myMother,
                   const int method,
                   const std::string& container)
    : m_parent(myMother)
    , m_extension(Particle2MCMethod::extension[method])
    , m_linkerAlgType(Particle2MCMethod::algType[method])
    , m_containerList(std::vector<std::string>(1,container))
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const GaudiTool* myMother,
                   const std::string& algType,
                   const std::string& extension,
                   std::vector<std::string> containerList)
    : m_parent(myMother)
    , m_extension(extension)
    , m_linkerAlgType(algType)
    , m_containerList( std::move(containerList) )
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const GaudiTool* myMother,
                   const std::string& algType,
                   const std::string& extension,
                   const std::string& container)
    : m_parent(myMother)
    , m_extension(extension)
    , m_linkerAlgType(algType)
    , m_containerList(std::vector<std::string>(1,container))
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}

  Object2MCLinker( const GaudiTool* myMother )
    : m_parent(myMother)
    , m_linkTo(myMother->evtSvc(),nullptr,"")
    , m_linkerTable(myMother->evtSvc(),nullptr,"") {}


  StatusCode setAlgorithm( const int method,
                           std::vector<std::string> containerList);

  StatusCode setAlgorithm( const int method,
                           const std::string& container)
  {
    return setAlgorithm( method, std::vector<std::string>(1,container));
  }

  StatusCode setAlgorithm( const std::string& algType,
                           const std::string& extension,
                           std::vector<std::string> containerList);

  StatusCode setAlgorithm( const std::string& algType,
                           const std::string& extension,
                           const std::string& container)
  {
    return setAlgorithm( algType, extension,
                         std::vector<std::string>(1,container));
  }



  typedef LinkedTo<LHCb::MCParticle>          To;

  typedef LinkerWithKey<LHCb::MCParticle, SOURCE>     Linker;

  Linker* linkerTable( const std::string& name) ;

  Linker* linkerTable( const std::string& name, To& test) ;

  ToRange rangeFrom(const SOURCE* part);

  const LHCb::MCParticle* firstMCP( const SOURCE* obj) ;

  const LHCb::MCParticle* firstMCP( const SOURCE* obj, double& weight) ;

  const LHCb::MCParticle* nextMCP() { return m_linkTo.next(); }

  const LHCb::MCParticle* nextMCP( double& weight) {
    const LHCb::MCParticle* mcPart =  m_linkTo.next();
    weight = ( mcPart ? m_linkTo.weight() : 0. );
    return mcPart;
  }

  double      weightMCP() { return m_linkTo.weight(); }

  int         associatedMCP( const SOURCE* obj) ;

  const LHCb::MCParticle* first ( const SOURCE* obj ) {
    return firstMCP( obj );
  }

  const LHCb::MCParticle* first ( const SOURCE* obj, double & weight ) {
    return firstMCP( obj, weight );
  }

  const LHCb::MCParticle* next() { return m_linkTo.next(); }

  const LHCb::MCParticle* next( double& weight) { return nextMCP(weight); }

  double weight() { return m_linkTo.weight(); }
  bool notFound();
  bool notFound( const std::string& contname);
  bool checkAssociation( const SOURCE* obj,
                         const LHCb::MCParticle* mcPart);

protected:
  const std::string& name() const
  { return boost::apply_visitor( [](const auto* p) ->decltype(auto) { return p->name(); }, m_parent ); }
  const std::string& context() const
  { return boost::apply_visitor( [](const auto* p) ->decltype(auto) { return p->context(); }, m_parent ); }
  StatusCode Error( const std::string& err, StatusCode sc)
  { return boost::apply_visitor( [&](const auto* p) { return p ? p->Error(err,sc) : sc ; }, m_parent) ; }
  StatusCode Warning( const std::string& err, StatusCode sc)
  { return boost::apply_visitor( [&](const auto* p) { return p ? p->Warning(err,sc) : sc ; }, m_parent); }
  MsgStream& debug()
  { return boost::apply_visitor( [](const auto* p) -> decltype(auto) { return p->debug(); }, m_parent ); }
  MsgStream& err()
  { return boost::apply_visitor( [](const auto* p) -> decltype(auto) { return p->err(); }, m_parent ); }
  MsgStream& error()
  { return boost::apply_visitor( [](const auto* p) -> decltype(auto) { return p->error(); }, m_parent ); }
  bool hasValidParent() const
  { return boost::apply_visitor( [](const auto* p) { return p!=nullptr; }, m_parent ); }

  IDataProviderSvc* evtSvc() const
  { return boost::apply_visitor( [](const auto* p) -> IDataProviderSvc* { return p->evtSvc(); }, m_parent ); }
  IMessageSvc*  msgSvc() const
  { return boost::apply_visitor( [](const auto* p) { return p->msgSvc(); }, m_parent ); }
  ISvcLocator*  svcLocator() const
  { return boost::apply_visitor( compose( [](const AlgTool* p) { return p->svcLoc(); },
                                          [](const Algorithm* p) { return p->svcLoc().get(); } ),
                                 m_parent ); }

  boost::variant<const GaudiAlgorithm*,const GaudiTool*> m_parent;
  std::string                  m_extension;
  std::string                  m_linkerAlgType;
  IAlgorithm*                  m_linkerAlg = nullptr;
  std::vector<std::string>     m_containerList;

  To                           m_linkTo;
  Linker                       m_linkerTable;

  // Private methods
  void createLinks( const std::string& contName = "") ;

  To* getLink( const std::string& contName ) ;

  StatusCode
  locateAlgorithm( const std::string& algType,
                   const std::string& algName,
                   IAlgorithm*& alg,
                   const std::vector<std::string>& inputData);
  std::string getGaudiRootInTES();
  StatusCode
  setAlgInputData( IAlgorithm*& alg,
                   const std::vector<std::string>& inputData);

  inline std::string
  containerName( const ContainedObject* obj) const
  {
    return ( obj->parent() && obj->parent()->registry() ) ?
                  obj->parent()->registry()->identifier() :
                  std::string{} ;
  }
};

template <class OBJ2MCP=LHCb::Particle>
class Object2FromMC : public Object2MCLinker<OBJ2MCP> {


public:
  /// Standard constructors
  Object2FromMC(const Algorithm* myMother)
    : Object2MCLinker<OBJ2MCP>( myMother )
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const Algorithm* myMother,
                 const int method,
                 std::vector<std::string> containerList)
    : Object2MCLinker<OBJ2MCP>( myMother, method, std::move(containerList))
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const Algorithm* myMother,
                 const int method,
                 const std::string& container )
    : Object2MCLinker<OBJ2MCP>( myMother, method, container)
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const Algorithm* myMother,
                 const std::string& algType,
                 const std::string& extension,
                 std::vector<std::string> containerList)
    : Object2MCLinker<OBJ2MCP>( myMother, algType, extension, std::move(containerList))
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const Algorithm* myMother,
                 const std::string& algType,
                 const std::string& extension,
                 const std::string& container)
    : Object2MCLinker<OBJ2MCP>( myMother, algType, extension, container)
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC(const GaudiTool* myMother)
    : Object2MCLinker<OBJ2MCP>( myMother )
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const GaudiTool* myMother,
                 const int method,
                 std::vector<std::string> containerList)
    : Object2MCLinker<OBJ2MCP>( myMother, method, std::move(containerList))
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const GaudiTool* myMother,
                 const int method,
                 const std::string& container )
    : Object2MCLinker<OBJ2MCP>( myMother, method, container)
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const GaudiTool* myMother,
                 const std::string& algType,
                 const std::string& extension,
                 std::vector<std::string> containerList)
    : Object2MCLinker<OBJ2MCP>( myMother, algType, extension, std::move(containerList))
    , m_linkFrom( m_linkFromList.end() ) {}

  Object2FromMC( const GaudiTool* myMother,
                 const std::string& algType,
                 const std::string& extension,
                 const std::string& container)
    : Object2MCLinker<OBJ2MCP>( myMother, algType, extension, container)
    , m_linkFrom( m_linkFromList.end() ) {}

  typedef LinkedFrom<OBJ2MCP>  From;

  bool        isAssociated( const KeyedObject<int>* obj)
  {
    const OBJ2MCP* part = dynamic_cast<const OBJ2MCP*>(obj);
    if( part ) return nullptr != firstMCP(part);
    const LHCb::MCParticle* mcPart = dynamic_cast<const LHCb::MCParticle*>(obj);
    if( mcPart ) return nullptr != firstP(mcPart);
    return false;
  }

  OBJ2MCP*     firstP( const LHCb::MCParticle* mcPart, double& weight)
  {
    OBJ2MCP* part = firstP(mcPart);
    weight = weightP();
    return part;
  }

  OBJ2MCP*     firstP( const LHCb::MCParticle* mcPart)
  {
    if ( !this->hasValidParent()
         || this->m_linkerAlgType.empty()
         || this->m_containerList.empty() ) return nullptr;

    createFromLinks();
    for( auto fr = m_linkFromList.begin();
         m_linkFromList.end() != fr; fr++) {
      OBJ2MCP* part = fr->first( mcPart );
      if( part ) {
        // Remember which table and which MCParticle we are dealing with...
        m_linkFrom = fr;
        m_linkFromMCP = mcPart;
        return part;
      }
    }
    m_linkFrom = m_linkFromList.end();
    m_linkFromMCP = nullptr;
    return nullptr;
  }

  OBJ2MCP*     nextP(double& weight)
  {
    OBJ2MCP* part = nextP();
    weight = ( part ? weightP() : 0. );
    return part;
  }

  OBJ2MCP*     nextP()
  {
    // If there was not a first() called before, stop there
    if( !m_linkFromMCP ) return nullptr;
    auto fr = m_linkFrom;
    if( m_linkFromList.end() == fr ) return nullptr;
    OBJ2MCP* part = fr->next();
    if( part ) return part;

    while( m_linkFromList.end() != ++fr ) {
      // Get the first Particle associated to the MCP we were dealing with
      OBJ2MCP* part = fr->first( m_linkFromMCP );
      if( part ) {
        m_linkFrom = fr;
        return part;
      }
    }
    m_linkFrom = m_linkFromList.end();
    m_linkFromMCP = nullptr;
    return nullptr;
  }

  double      weightP()
  {
    return m_linkFromList.end() != m_linkFrom ? m_linkFrom->weight() : 0.;
  }

  int         associatedP( const KeyedObject<int>* obj)
  {
    int n = 0;
    for( auto* part = firstP(obj); part; part = nextP() ) { ++n; }
    return n;
  }

private:
  std::vector<From>           m_linkFromList;
  typename std::vector<From>::iterator m_linkFrom;
  const LHCb::MCParticle*     m_linkFromMCP = nullptr;

  void        createFromLinks()
  {
    m_linkFromList.clear();
    for( const auto& cont : this->m_containerList ) {
      const std::string name = cont + this->m_extension;
      From test(  this->evtSvc(), nullptr, name);
      if( test.notFound() ) {
        this->createLinks( cont );
        test = From(  this->evtSvc(), nullptr, name);
      }
      m_linkFromList.emplace_back( std::move(test) );
    }
    m_linkFrom = m_linkFromList.end();
  }

};

#include "Particle2MCLinker.icpp"

/** Linker type for associations between ProtoParticles and MCParticles
 *
 *  @author Philippe Charpentier
 *  @date   2004-04-29
 */
typedef Object2FromMC<LHCb::ProtoParticle> ProtoParticle2MCLinker;

/** Linker type for associations between Particles and MCParticles
 *
 *  @author Philippe Charpentier
 *  @date   2004-04-29
 */
typedef Object2FromMC<>                    Particle2MCLinker;

#endif // DAVINCIASSOCIATORS_PARTICLE2MCLINKER_H
