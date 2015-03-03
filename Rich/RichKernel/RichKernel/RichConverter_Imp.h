
//-----------------------------------------------------------------------------
/** @file RichConverter_Imp.h
 *
 *  Implementation file for class : Rich::Converter_Imp
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2009-07-07
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHCONVERTERIMP_H
#define RICHKERNEL_RICHCONVERTERIMP_H 1

// Gaudi
#include "GaudiKernel/Converter.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/HashMap.h"
#include "GaudiKernel/StatEntity.h"

class IDataProviderSvc;
class IChronoStatSvc;

namespace Rich
{

  /** @class Converter_Imp RichKernel/RichConverter.h
   *
   *  Extends ::Converter class with GaudiAlg like methods
   *
   *  @author Chris Jones
   *  @date   2009-07-07
   */
  class Converter_Imp : public ::Converter
  {

  public:

    /// Standard constructor
    Converter_Imp ( long storage_type,
                    const CLID &class_type,
                    ISvcLocator *svc = NULL )
      : ::Converter ( storage_type, class_type, svc ),
      m_context     ( "Offline" ),
      m_msgStream   ( NULL ),
      m_toolSvc     ( NULL ),
      m_detSvc      ( NULL ),
      m_chronoSvc   ( NULL )
    {
      /// @todo make this work
      //declareProperty( "Context", m_context );
    }

    /// Destructor
    virtual ~Converter_Imp() { resetMsgStream(); }

    /// standard initialization method
    virtual StatusCode initialize();

    /// standard finalization  method
    virtual StatusCode finalize  ();

  public:

    /// name of this converter
    virtual const std::string & name() const { return m_name; }

  protected: // few actual data types

    /// the actual type of general counters
    typedef std::map<std::string,StatEntity>   Statistics ;
    /// the actual type error/warning counter
    typedef std::map<std::string,unsigned int> Counter    ;
    /// storage for active tools
    typedef std::vector<IAlgTool*>             AlgTools   ;
    /// storage for active services
    typedef GaudiUtils::HashMap<std::string, SmartIF<IService> > Services;

  public: // tools and services

    /// accessor to ToolSvc
    IToolSvc * toolSvc() const;

    /// accessor to Detector Data Svc
    IDataProviderSvc* detSvc() const;

    /// accessor to Chrono & Stat service
    IChronoStatSvc* chronoSvc() const;

    // Useful method for the easy location of tools.
    template < class TOOL >
    TOOL* tool ( const std::string& type           ,
                 const std::string& name    = ""   ,
                 const IInterface*  parent  = 0    ,
                 bool               create  = true ) const
    {
      TOOL* Tool = 0 ;
      if ( name.empty() )
      {
        this->toolSvc()->retrieveTool( type, Tool, parent, create );
      }
      else
      {
        this->toolSvc()->retrieveTool( type, name, Tool, parent, create );
      }
      // add the tool into list of known tools to be properly released
      addToToolList( Tool );
      return Tool;
    }

    template < class SERVICE >
    inline SmartIF<SERVICE> svc( const std::string& name   ,
                                 const bool         create = true ) const
    {
      SmartIF<IService> & baseSvc = this->serviceLocator()->service(name, create);
      // Try to get the requested interface
      SmartIF<SERVICE> s;
      s = baseSvc;
      // check the results
      if ( !baseSvc.isValid() || !s.isValid() )
      {
        this->Exception ("svc():: Could not retrieve Svc '" + name + "'", StatusCode::FAILURE);
      }
      // add the tool into list of known tools, to be properly released
      addToServiceList(baseSvc);
      // return *VALID* located service
      return s;
    }

    /// Release tools and services
    StatusCode release ( const IInterface* interface ) const;

    // ============================================================================
    // Add the given tool to the list of active tools
    // ============================================================================
    inline void addToToolList( IAlgTool * tool ) const
    {
      m_tools.push_back( tool ) ;
    }
    // ============================================================================

    // ============================================================================
    // Add the given service to the list of active services
    // ============================================================================
    inline void addToServiceList( const SmartIF<IService>& svc ) const
    {
      if (svc.isValid()) {
        m_services[svc->name()] = svc;
      }
    }
    // ============================================================================

  protected:

    /// manual forced (and 'safe') release of the tool
    StatusCode releaseTool ( const IAlgTool*   tool ) const ;

    /// manual forced (and 'safe') release of the service
    StatusCode releaseSvc  ( const IInterface* svc  ) const ;

  public: // messaging

    /** @brief Test the output level
     *  @param level The message level to test against
     *  @return boolean Indicating if messages at given level will be printed
     *  @retval true Messages at level "level" will be printed
     *  @retval true Messages at level "level" will NOT be printed
     */
    inline bool msgLevel( const MSG::Level /* level */ ) const
    {
      // return msgLevel() <= level ;
      /// @todo Implement properly
      return false;
    }

    /** Predefined configurable message stream for the efficient printouts
     *
     *  @code
     *
     *  if ( a < 0 ) { msgStream( MSG::ERROR ) << "a = " << endmsg ; }
     *
     *  @endcode
     *
     *  @return Reference to the predefined stream
     */
    inline MsgStream&
    msgStream ( const MSG::Level level ) const
    {
      if ( !m_msgStream )
      { m_msgStream = new MsgStream ( this->msgSvc() , this->name() ) ; }
      return *m_msgStream << level ;
    }

    /// Delete the current messaging object
    inline void resetMsgStream() const
    {
      delete m_msgStream; 
      m_msgStream = NULL;
    }

    /// shortcut for the method msgStream ( MSG::ALWAYS )
    inline MsgStream&  always () const { return msgStream ( MSG::ALWAYS ) ; }
    /// shortcut for the method msgStream ( MSG::FATAL   )
    inline MsgStream&   fatal () const { return msgStream ( MSG::FATAL ) ; }
    /// shortcut for the method msgStream ( MSG::ERROR   )
    inline MsgStream&     err () const { return msgStream ( MSG::ERROR ) ; }
    /// shortcut for the method msgStream ( MSG::ERROR   )
    inline MsgStream&   error () const { return msgStream ( MSG::ERROR ) ; }
    /// shortcut for the method msgStream ( MSG::WARNING )
    inline MsgStream& warning () const { return msgStream ( MSG::WARNING ) ; }
    /// shortcut for the method msgStream ( MSG::INFO    )
    inline MsgStream&    info () const { return msgStream ( MSG::INFO ) ; }
    /// shortcut for the method msgStream ( MSG::DEBUG   )
    inline MsgStream&   debug () const { return msgStream ( MSG::DEBUG ) ; }
    /// shortcut for the method msgStream ( MSG::VERBOSE )
    inline MsgStream& verbose () const { return msgStream ( MSG::VERBOSE ) ; }
    /// shortcut for the method msgStream ( MSG::INFO    )
    inline MsgStream&     msg () const { return msgStream ( MSG::INFO ) ; }

    /// print error message
    StatusCode Error     ( const std::string    & Message ,
                           const StatusCode       Status  = StatusCode::FAILURE ) const;

    /// print warning  message
    StatusCode Warning   ( const std::string    & Message                       ,
                           const StatusCode       Status  = StatusCode::FAILURE ) const;

    /// print info message
    StatusCode Info      ( const std::string    & Message ,
                           const StatusCode       Status  = StatusCode::SUCCESS ) const;

    /// print debug message
    StatusCode Debug     ( const std::string    & Message ,
                           const StatusCode       Status  = StatusCode::SUCCESS ) const;

    /// print verbose message
    StatusCode Verbose   ( const std::string    & Message ,
                           const StatusCode       Status  = StatusCode::SUCCESS ) const;

    /// print the message
    StatusCode Print     ( const std::string    & Message                       ,
                           const MSG::Level       level   = MSG::INFO           ,
                           const StatusCode       Status  = StatusCode::SUCCESS ) const;

    /// Throw an exception
    StatusCode Exception
    ( const std::string    & msg,
      const StatusCode       sc  = StatusCode(StatusCode::FAILURE,true) ) const
    {
      throw GaudiException( this->name() + ":: " + msg , "",  sc );
      return sc;
    }

  public:

    /// access context
    inline const std::string & context() const { return m_context; }

  protected:

    /// set new converter name
    inline void setName ( const std::string& newName )
    {
      m_name = newName;
    }

  protected:

    /** 'Dummy' implementation of property setting method
     *  @todo Extend to a full implementation if needed 
     */
    template < class TYPE >
    void declareProperty ( const std::string & /* name */,
                           TYPE & /* property */,
                           const std::string & /* doc = "none" */ ) { }

  private:

    /// name of this converter
    std::string m_name;

    /// Context like string
    std::string m_context;

    /// The predefined message stream
    mutable MsgStream* m_msgStream;

    /// Tool service
    mutable IToolSvc * m_toolSvc;

    /// detector data service
    mutable IDataProviderSvc* m_detSvc;

    /// Chrono & Stat service
    mutable IChronoStatSvc* m_chronoSvc;

    /// List of active  tools
    mutable AlgTools   m_tools;

    /// List of active  services
    mutable Services   m_services;

  };

}

#endif // RICHKERNEL_RICHCONVERTERIMP_H
