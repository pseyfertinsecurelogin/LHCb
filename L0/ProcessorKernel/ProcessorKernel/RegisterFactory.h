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

#ifndef PROCESSORKERNEL_REGISTERFACTORY_H
#define PROCESSORKERNEL_REGISTERFACTORY_H 1

#include "ProcessorKernel/Register.h"
#include "ProcessorKernel/TileRegister.h"
#include <map>
#include <string>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

namespace L0Muon {

  /** @class RegisterFactory RegisterFactory.h L0MuonKernel/RegisterFactory.h

  Class representing a factory for creating and owning registers

  @author  Andrei Tsaregorodtsev, Julien Cogan
  @date  12 January 2002
  */
  class RegisterFactory {

  protected:
    /// Constructor
    RegisterFactory();

  public:
    /// Destructor
    virtual ~RegisterFactory();

    /// single instance for the factory
    static RegisterFactory* instance();

    static void selectInstance( int i );

    /**
       Create a register.
       If the register was already created, it
       return the existing one.

       @param name : name of the register
       @param size : number of bits
    */
    Register* createRegister( std::string name, int size );
    Register* searchRegister( std::string name );

    /**
       Create a tileregister.
       If the register was alredy created, it return the
       existing one

       @param name : name of the register
       @param size : number of bits
    */
    TileRegister* createTileRegister( std::string name, int size );
    TileRegister* searchTileRegister( std::string name );

    /// Crate an alias for the name of the register
    int createAlias( std::string name, std::string alias );

    /// Delete the registers named name
    void                             deleteRegister( std::string name );
    std::map<std::string, Register*> getRegisters() { return m_registers; }

    /// True if the register is found
    bool found() { return m_found; }

    /// Dump the factory state
    void dump( bool full = false );

    /// Empty the factory
    void reset();

    /// Write out the XML file describing the registers
    std::string toXML( std::string tab = "" );

    /// Fill the factory from the XML file describing the registers
    void fromXML( DOMNode* pNode );
    void registerFromNode( DOMNode* pNode );
    void tileRegisterFromNode( DOMNode* pNode );
    void registerAliasFromNode( DOMNode* pNode );
    void tileFromNode( DOMNode* pNode, std::vector<LHCb::MuonTileID>* mids, boost::dynamic_bitset<>* tilestag,
                       boost::dynamic_bitset<>* stripstag );

    /// Flag indicating is the factory has already been filled from the xml file
    bool filledFromXML() { return m_filledFromXML; }

  private:
    /// Utilities for XML decoding: get the attribute key in the list di
    /// and convert it into an integer, a string,...
    int         getAttributeInt( DOMNamedNodeMap* di, const char* key );
    std::string getAttributeStr( DOMNamedNodeMap* di, const char* key );

  private:
    std::map<std::string, Register*> m_registers; // registers repository
    bool                             m_found;
    bool                             m_filledFromXML;
  };

} // namespace L0Muon

#endif // PROCESSORKERNEL_REGISTERFACTORY_H
