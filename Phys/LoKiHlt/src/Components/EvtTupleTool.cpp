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
// ============================================================================
// Include files
// ============================================================================
#include "boost/algorithm/string/join.hpp"
#include "boost/algorithm/string/predicate.hpp"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// ============================================================================
// DaVinciInterfaces
// ============================================================================
#include "Kernel/IEventTupleTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ICoreFactory.h" // void & numbers
#include "LoKi/IHltFactory.h"  // ODIN, HDR & L0
// ============================================================================
// Event
// ============================================================================
#include "Event/HltDecReports.h"
#include "Event/L0DUReport.h"
#include "Event/ODIN.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Hybrid {
    // ========================================================================
    /** @class EvtTupleTool
     *  Simple LoKi-Hybryd tool for "event"-level information
     *
     *   - ODIN           -> double
     *   - L0DUReport     -> double
     *   - HltDecReports  -> double
     *   - void           -> double
     *
     *
     *  @code
     *
     *    XXXTool.ODIN_Variables =
     *    {
     *      "Run" : "ODIN_RUN"
     *    }
     *
     *    XXXTool.ODIN_Variables =
     *    {
     *      "L0" : " switch ( L0_DECISION , 1 ,  0 ) "
     *    }
     *
     *    XXXTool.HLT_Variables =
     *    {
     *      "size" : " HLT_SIZE "
     *    }
     *
     *    XXXTool.VOID_Variables =
     *    {
     *      "nPV" : " CONTAINS ( 'Rex/Vertex/Primary') "
     *    }
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date 2010-02-16
     */
    class EvtTupleTool : public GaudiTool, public virtual IEventTupleTool {
    public:
      // ======================================================================
      /**  Fill the tuple
       *   @see IEventTupleTool
       */
      StatusCode fill( Tuples::Tuple& t ) override;
      // ======================================================================
    public:
      // ======================================================================
      /// standard initialization of the tool
      StatusCode initialize() override;
      /// standard finalization   of the tool
      StatusCode finalize() override;
      /** constructor
       *  @param type   the tool type (??)
       *  @param name   the tool instance name
       *  @param parent the tool parent
       */
      EvtTupleTool( const std::string& type,    // tool type ??
                    const std::string& name,    // tool instance name
                    const IInterface*  parent ); // the parent
      // ======================================================================
    public:
      // ======================================================================
      /// the update handler for 'Preambulo'
      void handlePreambulo( Property& /* p */ );
      /// the update handler for 'CoreFactory'
      void handleCore( Property& /* p */ );
      /// the update handler for 'HltFactory'
      void handleHlt( Property& /* p */ );
      /// the update handler for ODIN_Variables
      void handleODIN( Property& /* p */ );
      /// the update handler for L0DU_Variables
      void handleL0DU( Property& /* p */ );
      /// the update handler for  HLT_Variables
      void handleHLT( Property& /* p */ );
      /// the update handler for VOID_Variables
      void handleVOID( Property& /* p */ );
      // ======================================================================
    protected:
      // ======================================================================
      /// update ODIN-variables
      StatusCode updateODIN(); // update ODIN-variables
      /// update L0DU-variables
      StatusCode updateL0DU(); // update L0DU-variables
      /// update HLT-variables
      StatusCode updateHLT(); // update  HLT-variables
      /// update VOID-variables
      StatusCode updateVOID(); // update VOID-variables
      // ======================================================================
    protected:
      // ======================================================================
      /// the preambulo
      std::string preambulo() const { return boost::algorithm::join( m_preambulo.value(), "\n" ); }
      // ======================================================================
    protected:
      // ======================================================================
      /** helper class to keep the N-tuple items
       *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
       *  @date 2010-02-16
       */
      template <class TYPE>
      struct Item {
        // ====================================================================
        /// the variable name
        std::string m_name; // the variable name
        /// the functor itself
        typename LoKi::BasicFunctors<TYPE>::FunctionFromFunction m_fun{
            typename LoKi::BasicFunctors<TYPE>::Constant( -1.e+10 )};
        // ====================================================================
      };
      // ======================================================================
      /// the actual type of {"name":"functor"} map
      typedef std::map<std::string, std::string> Map;
      /// the actual type of containter of items
      typedef std::vector<Item<const LHCb::ODIN*>> ODIN_Items;
      /// the actual type of containter of items
      typedef std::vector<Item<const LHCb::L0DUReport*>> L0_Items;
      /// the actual type of containter of items
      typedef std::vector<Item<const LHCb::HltDecReports*>> HLT_Items;
      /// the actual type of containter of items
      typedef std::vector<Item<void>> VOID_Items;
      // ======================================================================
    private:
      // ======================================================================
      /// Preambulo:
      Gaudi::Property<std::vector<std::string>> m_preambulo{
          this,
          "Preambulo",
          {},
          &LoKi::Hybrid::EvtTupleTool::handlePreambulo,
          "The preambulo lines to be used for the temporary python script"};
      /// CoreFactory name
      Gaudi::Property<std::string> m_core{this, "CoreFactory", "LoKi::Hybrid::CoreFactory/CoreFactory:PUBLIC",
                                          &LoKi::Hybrid::EvtTupleTool::handleCore,
                                          "The type/name of LoKiBender \"hybrid\" factory: void->double"};
      /// HltFactory  name
      Gaudi::Property<std::string> m_hlt{
          this, "HltFactory", "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC", &LoKi::Hybrid::EvtTupleTool::handleHlt,
          "The type/name of LoKiBender \"hybrid\" factory: ODIN->double, L0DUReport->double & HltDecReports->double"};
      // ======================================================================
      /// the { "name" : "functor" } map ( property)
      Gaudi::Property<Map> m_map_odin // the { "name" : "functor" } map ( property)
          {this,
           "ODIN_Variables",
           {},
           &LoKi::Hybrid::EvtTupleTool::handleODIN,
           "The map { 'name' : 'functor'} of variables: LHCb::ODIN -> double "};
      /// the { "name" : "functor" } map ( property)
      Gaudi::Property<Map> m_map_l0 // the { "name" : "functor" } map ( property)
          {this,
           "L0DU_Variables",
           {},
           &LoKi::Hybrid::EvtTupleTool::handleL0DU,
           "The map { 'name' : 'functor'} of variables: LHCb::L0DUReport -> double   "};
      /// the { "name" : "functor" } map ( property)
      Gaudi::Property<Map> m_map_hdr // the { "name" : "functor" } map ( property)
          {this,
           "HLT_Variables",
           {},
           &LoKi::Hybrid::EvtTupleTool::handleHLT,
           "The map { 'name' : 'functor'} of variables:  LHCb::HltDecReports-> double "};
      /// the { "name" : "functor" } map ( property)
      Gaudi::Property<Map> m_map_void // the { "name" : "functor" } map ( property)
          {this,
           "VOID_Variables",
           {},
           &LoKi::Hybrid::EvtTupleTool::handleVOID,
           "The map { 'name' : 'functor'} of variables: void -> double "};
      // ======================================================================
    private:
      // ======================================================================
      /// the items: ODIN
      ODIN_Items m_odin; // the items: ODIN
      /// the items: L0
      L0_Items m_l0; // the items:   L0
      /// the items: HLT
      HLT_Items m_hdr; // the items:  HLT
      /// the items: void
      VOID_Items m_void; // the items: void
      // ======================================================================
    private:
      // ======================================================================
      /// the TES-location of ODIN
      DataObjectReadHandle<LHCb::ODIN> m_ODINLocation //          the TES-location of ODIN
          {this, "ODIN_Location", LHCb::ODINLocation::Default, "TES-location of LHCb::ODIN object"};
      /// the TES-location of L0DUReports
      DataObjectReadHandle<LHCb::L0DUReport> m_L0DULocation //   the TES-location of L0DUReports
          {this, "L0DU_Location", LHCb::L0DUReportLocation::Default, "TES-location of LHCb::L0DUReport object"};
      /// the TES-location of HltDecReports
      DataObjectReadHandle<LHCb::HltDecReports> m_HDRLocation // the TES-location of HltDecReports
          {this, "HLT_Location", LHCb::HltDecReportsLocation::Default, "TES-location of LHCb::HltDecReports object"};
      // ======================================================================
    };
    // ========================================================================
  } // namespace Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
/*  constructor
 *  @param type   the tool type (??)
 *  @param name   the tool instance name
 *  @param parent the tool parent
 */
// ============================================================================
LoKi::Hybrid::EvtTupleTool::EvtTupleTool( const std::string& type,   // tool type
                                          const std::string& name,   // tool instance name
                                          const IInterface*  parent ) // the parent
    : GaudiTool( type, name, parent ) {
  // interface
  declareInterface<IEventTupleTool>( this );
  //
  if ( boost::algorithm::starts_with( name, "Hlt1" ) ) {
    m_core = "LoKi::Hybrid::CoreFactory/Hlt1CoreFactory:PUBLIC";
  } else if ( boost::algorithm::starts_with( name, "Hlt2" ) ) {
    m_core = "LoKi::Hybrid::CoreFactory/Hlt2CoreFactory:PUBLIC";
  }
  //
  if ( boost::algorithm::starts_with( name, "Hlt1" ) ) {
    m_hlt = "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC";
  } else if ( boost::algorithm::starts_with( name, "Hlt2" ) ) {
    m_hlt = "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC";
  }
}
// ============================================================================
// standard initialization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if ( sc.isFailure() ) { return sc; }
  svc<ILoKiSvc>( "LoKiSvc", true );
  // decode variables:
  if ( !m_map_odin.empty() ) {
    sc = updateODIN();
    if ( sc.isFailure() ) { return Error( "Unable to decode ODIN_Variables", sc ); }
  }
  // ==========================================================================
  if ( !m_map_l0.empty() ) {
    sc = updateL0DU();
    if ( sc.isFailure() ) { return Error( "Unable to decode L0DU_Variables", sc ); }
  }
  // ==========================================================================
  if ( !m_map_hdr.empty() ) {
    sc = updateHLT();
    if ( sc.isFailure() ) { return Error( "Unable to decode  HLT_Variables", sc ); }
  }
  // ==========================================================================
  if ( !m_map_void.empty() ) {
    sc = updateVOID();
    if ( sc.isFailure() ) { return Error( "Unable to decode VOID_Variables", sc ); }
  }
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// standard finalization   of the tool
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::finalize() {
  m_odin.clear();
  m_l0.clear();
  m_hdr.clear();
  m_void.clear();
  //
  return GaudiTool::finalize();
}
// ============================================================================
// the update handler for Preambulo
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handlePreambulo( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of Preambulo" ).ignore();
  //
  StatusCode sc = updateODIN();
  Assert( sc.isSuccess(), "Unable to set 'ODIN_Variables'", sc );
  sc = updateL0DU();
  Assert( sc.isSuccess(), "Unable to set 'L0DU_Variables'", sc );
  sc = updateHLT();
  Assert( sc.isSuccess(), "Unable to set  'HLT_Variables'", sc );
  sc = updateVOID();
  Assert( sc.isSuccess(), "Unable to set 'VOID_Variables'", sc );
}
// ============================================================================
// the update handler for HltFactory
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleHlt( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of HltFactory" ).ignore();
  //
  StatusCode sc = updateODIN();
  Assert( sc.isSuccess(), "Unable to set 'ODIN_Variables'", sc );
  sc = updateL0DU();
  Assert( sc.isSuccess(), "Unable to set 'L0DU_Variables'", sc );
  sc = updateHLT();
  Assert( sc.isSuccess(), "Unable to set  'HLT_Variables'", sc );
}
// ============================================================================
// The update handler for CoreFactory
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleCore( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of CoreFactory" ).ignore();
  //
  StatusCode sc = updateVOID();
  Assert( sc.isSuccess(), "Unable to set 'VOID_Variables'", sc );
}
// ============================================================================
// The update handler for VOID_Variables
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleVOID( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of VOID_Varaibles" ).ignore();
  //
  StatusCode sc = updateVOID();
  Assert( sc.isSuccess(), "Unable to set 'VOID_Variables'", sc );
}
// ============================================================================
// The update handler for ODIN_Variables
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleODIN( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of ODIN_Varaibles" ).ignore();
  //
  StatusCode sc = updateODIN();
  Assert( sc.isSuccess(), "Unable to set 'ODIN_Variables'", sc );
}
// ============================================================================
// The update handler for L0DU_Variables
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleL0DU( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of L0DU_Varaibles" ).ignore();
  //
  StatusCode sc = updateL0DU();
  Assert( sc.isSuccess(), "Unable to set 'L0DU_Variables'", sc );
}
// ============================================================================
// The update handler for HLT_Variables
// ============================================================================
void LoKi::Hybrid::EvtTupleTool::handleHLT( Property& /* p */ ) {
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return; }
  //
  Warning( "Reintialization of HLT_Variables" ).ignore();
  //
  StatusCode sc = updateHLT();
  Assert( sc.isSuccess(), "Unable to set  'HLT_Variables'", sc );
}
// ============================================================================
// update ODIN-variables
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::updateODIN() // update ODIN-variables
{
  // get the factory
  IHltFactory* factory = tool<IHltFactory>( m_hlt, this );
  //
  m_odin.clear();
  m_odin.reserve( m_map_odin.size() );
  for ( Map::const_iterator ivar = m_map_odin.begin(); m_map_odin.end() != ivar; ++ivar ) {
    Item<const LHCb::ODIN*> item;
    StatusCode              sc = factory->get( ivar->second, item.m_fun, preambulo() );
    if ( sc.isFailure() ) { return Error( "Unable to decode " + ivar->first + " : " + ivar->second, sc ); }
    //
    item.m_name = ivar->first;
    m_odin.push_back( item );
    //
    debug() << "The decoded variable name is '" << m_odin.back().m_name << "'\t, the functor : '" << m_odin.back().m_fun
            << "'" << endmsg;
  }
  //
  release( factory ); // we do not need the factory anymore
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// update L0DU-variables
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::updateL0DU() // update ODIN-variables
{
  // get the factory
  IHltFactory* factory = tool<IHltFactory>( m_hlt, this );
  //
  m_l0.clear();
  m_l0.reserve( m_map_l0.size() );
  for ( const auto& ivar : m_map_l0 ) {
    Item<const LHCb::L0DUReport*> item;
    StatusCode                    sc = factory->get( ivar.second, item.m_fun, preambulo() );
    if ( sc.isFailure() ) { return Error( "Unable to decode " + ivar.first + " : " + ivar.second, sc ); }
    //
    item.m_name = ivar.first;
    m_l0.push_back( std::move( item ) );
    //
    debug() << "The decoded variable name is '" << m_l0.back().m_name << "'\t, the functor : '" << m_l0.back().m_fun
            << "'" << endmsg;
  }
  //
  release( factory ); // we do not need the factory anymore
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// update  HLT-variables
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::updateHLT() // update HLT-variables
{
  // get the factory
  IHltFactory* factory = tool<IHltFactory>( m_hlt, this );
  //
  m_hdr.clear();
  m_hdr.reserve( m_map_hdr.size() );
  for ( const auto& ivar : m_map_hdr ) {
    Item<const LHCb::HltDecReports*> item;
    StatusCode                       sc = factory->get( ivar.second, item.m_fun, preambulo() );
    if ( sc.isFailure() ) { return Error( "Unable to decode " + ivar.first + " : " + ivar.second, sc ); }
    //
    item.m_name = ivar.first;
    m_hdr.push_back( std::move( item ) );
    //
    debug() << "The decoded variable name is '" << m_hdr.back().m_name << "'\t, the functor : '" << m_hdr.back().m_fun
            << "'" << endmsg;
  }
  //
  release( factory ); // we do not need the factory anymore
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// update  Void-variables
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::updateVOID() // update VOID-variables
{
  // get the factory
  ICoreFactory* factory = tool<ICoreFactory>( m_core, this );
  //
  m_void.clear();
  m_void.reserve( m_map_void.size() );
  for ( const auto& ivar : m_map_void ) {
    Item<void> item;
    StatusCode sc = factory->get( ivar.second, item.m_fun, preambulo() );
    if ( sc.isFailure() ) { return Error( "Unable to decode " + ivar.first + " : " + ivar.second, sc ); }
    //
    item.m_name = ivar.first;
    m_void.push_back( std::move( item ) );
    //
    debug() << "The decoded variable name is '" << m_void.back().m_name << "'\t, the functor : '" << m_void.back().m_fun
            << "'" << endmsg;
  }
  //
  release( factory ); // we do not need the factory anymore
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
/*  Fill the tuple
 *   @see IEventTupleTool
 */
// ============================================================================
StatusCode LoKi::Hybrid::EvtTupleTool::fill( Tuples::Tuple& t ) {
  if ( !t.valid() ) { return Error( "Tuples::Tuple object is invalid!" ); }
  //
  StatusCode sc = StatusCode( StatusCode::SUCCESS, true );
  // ==========================================================================
  // ODIN
  if ( !m_odin.empty() ) {
    const LHCb::ODIN* odin = m_ODINLocation.get();
    //
    for ( const auto& item : m_odin ) {
      StatusCode code = t->column( item.m_name, item.m_fun( odin ) );
      if ( code.isFailure() ) {
        Warning( "Error in column('" + item.m_name + "')", code ).ignore();
        sc = code;
      }
    }
  }
  // ==========================================================================
  // L0
  if ( !m_l0.empty() ) {
    const LHCb::L0DUReport* l0 = m_L0DULocation.get();
    //
    for ( const auto& item : m_l0 ) {
      StatusCode code = t->column( item.m_name, item.m_fun( l0 ) );
      if ( code.isFailure() ) {
        Warning( "Error in column('" + item.m_name + "')", code ).ignore();
        sc = code;
      }
    }
  }
  // ==========================================================================
  // HLT
  if ( !m_hdr.empty() ) {
    const LHCb::HltDecReports* hdr = m_HDRLocation.get();
    //
    for ( const auto& item : m_hdr ) {
      StatusCode code = t->column( item.m_name, item.m_fun( hdr ) );
      if ( code.isFailure() ) {
        Warning( "Error in column('" + item.m_name + "')", code ).ignore();
        sc = code;
      }
    }
  }
  // ==========================================================================
  // VOID
  for ( const auto& item : m_void ) {
    StatusCode code = t->column( item.m_name, item.m_fun() );
    if ( code.isFailure() ) {
      Warning( "Error in column('" + item.m_name + "')", code ).ignore();
      sc = code;
    }
  }
  //
  return sc;
}
// ============================================================================

// ============================================================================
/// Declaration of the Tool Factory
DECLARE_COMPONENT( LoKi::Hybrid::EvtTupleTool )
// ============================================================================

// ============================================================================
// The END
// ============================================================================
