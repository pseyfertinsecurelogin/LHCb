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
#ifndef L0MUONKERNEL_L0MUNIT_H
#define L0MUONKERNEL_L0MUNIT_H 1

#include "Kernel/MuonTileID.h"
#include "ProcessorKernel/Unit.h"

#include "L0MuonKernel/L0MPtLUT.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

namespace L0Muon {

  // The XML keywords

  /// The XML keyword for Property
  inline const std::string XMLProperty = "Property";

  /// The XML keyword for RegisterRecord
  inline const std::string XMLRegisterRecord = "RegisterRecord";

  /// The XML keyword for InputRegisters
  inline const std::string XMLInputRegisters = "InputRegisters";

  /// The XML keyword for OutputRegisters
  inline const std::string XMLOutputRegisters = "OutputRegisters";

  /// The XML keyword for Unit
  inline const std::string XMLUnit = "Unit";

  /** @class L0MUnit L0MUnit.h L0MuonKernel/L0MUnit.h

  Specialisation of the Unit class for the L0Muon processor.

  It implements :
  - a MuonTileID identifier (new attribute)
  - instanciation(/saving) from(/to) xml representation

  @author Julien Cogan
  @date  July the 1st 2005
  */
  class L0MUnit : public Unit {

  public:
    /// Constructor
    L0MUnit();

    /** Constructor with MuonTileID

        @param id : MuonTileID identifying the Unit
    */
    L0MUnit( LHCb::MuonTileID id );

    /** Constructor from XML (DOM)

        @param pNode : pointer to the DOMNode containing the Unit in the xml representation
    */
    L0MUnit( DOMNode* pNode );

    /// Destructor
    ~L0MUnit();

    /// Returns the MuonTileID identifier of the Unit.
    LHCb::MuonTileID mid() { return m_mid; }

    /// Save the Unit in XML format.
    std::string toXML( std::string tab = "" );

    /// Set the pointer to the pt Look Up Table
    virtual void setLUTPointer( L0MPtLUT* plut );

  protected:
    /// MuonTileID identifying the Unit
    LHCb::MuonTileID m_mid;

    /** Utility for XML decoding: get the attribute key in the list di
        and convert it into an integer
    */
    int getAttributeInt( DOMNamedNodeMap* di, const char* key );

    /** Utility for XML decoding: get the attribute key in the list di
        and convert it into a string
    */
    std::string getAttributeStr( DOMNamedNodeMap* di, const char* key );

    /// Utility for XML decoding: get the Unit property from the given node
    void propertyFromNode( DOMNode* pNode );

    /** Utility for XML decoding: get the list of registers from the given node
        @param io : 0 for input registers, 1 for output registers
    */
    void registersFromNode( DOMNode* pNode, int io );

    /// Utility for XML decoding: get the child Unit from the given node
    void unitFromNode( DOMNode* pNode );

  private:
    /// Pointer to the pt Look Up Table
    L0MPtLUT* m_plut;
  };

} // namespace L0Muon

#endif // L0MUONKERNEL_L0MUNIT_H
