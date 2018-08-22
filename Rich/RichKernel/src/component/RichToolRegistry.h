
//-----------------------------------------------------------------------------
/** @file RichToolRegistry.h
 *
 *  Header file for tool : Rich::ToolRegistry
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <map>
#include <sstream>
#include <string>

// base class and interface
#include "GaudiAlg/GaudiTool.h"
#include "RichInterfaces/IRichToolRegistry.h"

// Utils
#include "RichUtils/RichHashMap.h"

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class ToolRegistry RichToolRegistry.h
   *
   *  Tool providing a mapping between tool "nicknames" and types
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */
  //-----------------------------------------------------------------------------

  class ToolRegistry final : public GaudiTool, virtual public IToolRegistry
  {

  private: // definitions

    /// typedef of container of strings for job options
    using ToolList = std::vector< std::string >;

    /// typedef for the mapping between nicknames and class names
    using RichToolMap = Rich::HashMap< std::string, std::string >;

  public: // for Gaudi framework

    /// Standard constructor
    ToolRegistry( const std::string &type, const std::string &name, const IInterface *parent );

    // Initialization of the tool after creation
    StatusCode initialize() override final;

  public: // methods (and doxygen comments) inherited from interface

    // Converts a tool nickname into a particular class name
    const std::string &toolType( const std::string &nickname ) const override final;

    // Converts a tool "nickname" into a particular instance name
    const std::string toolName( const std::string &nickname ) const override final;

  private: // methods

    /// Set up the tool mappings
    void setUpTools( const ToolList &toolList );

    /// Adds a entry to the map between nicknames and class names
    void addEntry( const std::string &nickname, ///< tool nickname
                   const std::string &type      ///< tool class name
                   ) const;

  private: // data

    /// Tool data from job options
    ToolList m_names;

    /// The mapping between the tool name and type
    mutable RichToolMap m_myTools;
  };

} // namespace Rich
