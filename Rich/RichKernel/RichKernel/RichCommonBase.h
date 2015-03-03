
//-----------------------------------------------------------------------------
/** @file RichCommonBase.h
 *
 *  Header file for RICH base class : Rich::CommonBase
 *
 *  CVS Log :-
 *  $Id: RichCommonBase.h,v 1.17 2009-09-24 12:35:33 jonrob Exp $
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2005-08-27
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHCOMMONBASE_H
#define RICHKERNEL_RICHCOMMONBASE_H 1

// Interfaces
#include "RichKernel/IRichToolRegistry.h"

// Gaudi
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/DataObject.h"

// Forward declarations
class IJobOptionsSvc;

namespace Rich
{
  using GaudiUtils::operator<<;

  //-----------------------------------------------------------------------------
  /** @class CommonBase RichCommonBase.h RichKernel/RichCommonBase.h
   *
   *  Base class providing common functionality for all RICH tools and algorithms
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-08-27
   */
  //-----------------------------------------------------------------------------

  template <class PBASE>
  class CommonBase : public PBASE
  {

  public:

    /// Standard algorithm-like constructor
    CommonBase( const std::string& name,
                ISvcLocator* pSvcLocator );

    /// Standard tool-like constructor
    CommonBase( const std::string& type,
                const std::string& name,
                const IInterface* parent );

    /// Standard Converter-like Constructor
    CommonBase( long storage_type, 
                const CLID &class_type, 
                ISvcLocator *svc = NULL );

    /// Destructor
    virtual ~CommonBase( );

  public:

    /** Initialization of the algorithm after creation
     *
     * @return The status of the initialization
     * @retval StatusCode::SUCCESS Initialization was successful
     * @retval StatusCode::FAILURE Initialization failed
     */
    virtual StatusCode initialize();

    /** Finalization of the algorithm before deletion
     *
     * @return The status of the finalization
     * @retval StatusCode::SUCCESS Finalization was successful
     * @retval StatusCode::FAILURE Finalization failed
     */
    virtual StatusCode finalize();

  public:

    /** Returns the full location of the given object in the Data Store
     *
     *  @param pObj Data object
     *
     *  @return Location of given data object
     */
    inline std::string objectLocation( const DataObject * pObj ) const
    {
      return ( !pObj ? "Null DataObject !" :
               (pObj->registry() ? pObj->registry()->identifier() : "UnRegistered") );
    }

    /** @brief Returns a pointer to the tool associated to a given nickname.
     *
     *  Optionally also allows a particular instance name to be given.
     *
     *  Uses the RichToolRegistry tool to convert tool nicknames
     *  in the appropriate class name.
     *
     *  @param nickName   The nickname of the requested tool
     *  @param iName      The instance name of the requested tool
     *  @param pTool      Returned pointer to the requested tool
     *  @param parent     Pointer to parent (used to access private tools)
     *  @param commonTool Flags if this tool should be acquired as common (true) or
     *                    via the tool registry as a context specifc tool (false)
     *
     *  @return Pointer to the tool associated to the given nickname
     */
    template <typename TOOL>
    inline const TOOL* acquireTool( const std::string & nickName,
                                    const std::string & iName,
                                    const TOOL*& pTool,
                                    const IInterface * parent = 0,
                                    const bool commonTool = false ) const
    {
      // Check consistency
      if ( parent && commonTool )
      {
        this -> Error( "Tool " + nickName + " cannot be common and private !" ).ignore();
        return NULL;
      }

      // Construct name
      const std::string fullname =
        ( commonTool || parent ? iName : this->toolRegistry()->toolName(iName) );

      // If not private tool - Check Context and OutputLevel option
      if ( !parent )
      {
        if ( ! this -> setProperties(this->toolRegistry()->toolName(iName)) ) { return NULL; }
      }

      // get tool
      pTool =
        this -> template tool<TOOL>( this->toolRegistry()->toolType(nickName),
                                     fullname,
                                     parent );
      if ( !pTool )
      {
        this->Exception( "Null Pointer returned by ToolSvc for "+fullname );
      }
      else if ( this -> msgLevel(MSG::DEBUG) )
      {
        this -> debug() << " Acquired tool '" << pTool->name()
                        << "' of type '" << this->toolRegistry()->toolType(nickName) << "'" << endmsg;
      }

      // return the tool pointer
      return pTool;
    }

    /** @brief Returns a pointer to the tool associated to a given nickname.
     *
     *  Uses the RichToolRegistry tool to convert tool nicknames
     *  in the appropriate class name.
     *
     *  @param nickName   The nickname of the requested tool
     *  @param pTool      Returned pointer to the requested tool
     *  @param parent     Pointer to parent (used to access private tools)
     *  @param commonTool Flags if this tool should be acquired as common (true) or
     *                    via the tool registry as a context specifc tool (false)
     *
     *  @return Pointer to the tool associated to the given nickname
     */
    template <typename TOOL>
    inline const TOOL* acquireTool( const std::string & nickName,
                                    const TOOL*& pTool,
                                    const IInterface * parent = 0,
                                    const bool commonTool = false ) const
    {
      return this -> acquireTool( nickName, nickName, pTool, parent, commonTool );
    }

    /** @brief Forced release of a particular tool
     *
     *  Tools are automatically released during finalisation, so this method
     *  only need be used to release a tool early, before finalisation.
     *
     *  @param pTool  Pointer to the tool to be released
     */
    template <typename TOOL>
    inline void releaseTool( TOOL *& pTool ) const
    {
      if ( pTool )
      {
        if ( this -> msgLevel(MSG::DEBUG) )
        {
          this -> debug() << " Forced release for tool '" << pTool->name() << "'" << endmsg;
        }
        this -> release( pTool );
        pTool = NULL;
      }
      else
      {
        this -> Warning ( "Attempt to release a NULL Tool pointer" );
      }
    }

  protected: // methods

    /** Returns pointer to RICH tool registry tool
     *  Used internally by base class to convert tool nicknames
     *  in the appropriate class name
     *
     *  @return Pointer to the IRichToolRegistry interface
     */
    inline const Rich::IToolRegistry * toolRegistry() const
    {
      if ( !m_toolReg )
      {
        m_toolReg =
          this -> template tool < IToolRegistry > ( "Rich::ToolRegistry", m_regName );
      }
      return m_toolReg;
    }

    /// Pointer to Job Options Service
    IJobOptionsSvc* joSvc() const;

    /// String matching on context()
    bool contextContains( const std::string & str ) const;

    /** Generate a context specific TES location for the given Default location
     *  @param loc The Default TES location
     *  @return The context specific TES location for the input location
     */
    std::string contextSpecificTES( const std::string & loc ) const;

  private: // private methods

    /** Finds the propert object of a given name, for the given component name
     *
     *   @param name Component name
     *   @param property Property name
     *
     *   @return Pointer to the Property object if it exists
     */
    template < class PROPERTYTYPE >
    const PROPERTYTYPE * my_getProperty( const std::string & name,
                                         const std::string & property ) const;

    /** @brief Set the given Property for given public tool
     *
     *  Set the Property for public tools that do not explicitly 
     *  have it set. Uses the same settings as for the tool registry.
     *
     *  @param name     Tool name
     *  @param property The property name
     *
     *  @return Boolean indicating if setting was successful or not
     *  @retval TRUE  Setting was successful
     *  @retval FALSE Setting failed
     */
    template < class PROPERTYTYPE >
    bool my_setToolProperty( const std::string & name,
                             const std::string & property ) const;
    
    /** @brief Set the properties for given public tool
     *
     *  Private solution to the problem that properties like "Context" 
     *  are not set for public tools. This solution uses the properties as 
     *  defined by the Tool registry to set them for all public tools.
     *
     *  @param name Tool name
     *
     *  @return Boolean indicating if setting was successful or not
     *  @retval TRUE  Setting was successful
     *  @retval FALSE Setting failed
     */
    bool setProperties( const std::string & name ) const;
    
  private: // data

    /// Pointer to tool registry
    mutable const IToolRegistry * m_toolReg;

    /// Pointer to job options service
    mutable IJobOptionsSvc * m_jos;

    /// Runtime name for RichToolRegistry
    std::string m_regName;

  };

}

#endif // RICHKERNEL_RICHCOMMONBASE_H
