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

#ifndef PROCESSORKERNEL_UNIT_H
#define PROCESSORKERNEL_UNIT_H 1

#include "ProcessorKernel/Property.h"
#include "ProcessorKernel/Register.h"
#include <map>
#include <string>

namespace L0Muon {

  /** @class Unit Unit.h L0MuonKernel/Unit.h

  Class representing a unit of data processing logic
  of the level-0 muon trigger for hardware simulations

  @author  Andrei Tsaregorodtsev, Julien Cogan
  @date  12 January 2002
  */
  class Unit {

  public:
    /// Default constructor
    Unit();

    /// Destructor
    virtual ~Unit();

    /// Reset input/output registers
    void releaseRegisters();

    /// Reset output registers
    void releaseOutputRegisters();

    /// Reset output registers
    void releaseInputRegisters();

    /// Print the registers contents
    void dumpRegisters();

    /// set the pointer to the parent unit
    virtual void setParent( L0Muon::Unit* unit );

    /// return the parent unit
    Unit* parent() { return m_parent; }

    /// return the parent of the given type
    Unit* parentByType( std::string type );

    /// Return subunits
    std::vector<L0Muon::Unit*> units() { return m_units; }

    //     /// Search for subunit
    //     Unit * subUnit(std::string type);

    /// Add input register
    virtual void addInputRegister( L0Muon::Register* in );

    /** Add input register

    @param in      : pointer to the input register
    @param rname   : name of the input register
    */
    virtual void addInputRegister( L0Muon::Register* in, std::string rname );

    /// Add output register
    virtual void addOutputRegister( L0Muon::Register* out );

    /** Add output register

    @param out     : pointer to the output register
    @param rname   : name of the output register
    */
    virtual void addOutputRegister( L0Muon::Register* out, std::string rname );

    /// Add subunit
    void addUnit( L0Muon::Unit* unit );

    /// Contains sub units or not
    bool isEmpty() { return m_units.empty(); }

    /// Set the DEBUG level flag
    virtual void setDebugMode( bool debug = true );

    /// Virtual method to initialize the hierarchy of units
    virtual void initialize();

    /// Virtual method to preexecute the hierarchy of units
    virtual void preexecute();

    /// Virtual method to execute the hierarchy of units
    virtual void execute();

    /// Virtual method to postexecute the hierarchy of units
    virtual void postexecute();

    /// Virtual method to finalize the hierarchy of units
    virtual void finalize();

    /// Get the unit type
    virtual std::string type() = 0;

    /*   /// Set the unit name */
    /*   void setName(std::string uname) { m_name = uname; } */

    /*   /// Get the unit name */
    /*   std::string name() { return m_name; } */

    /// Print out the unit definition - to be overloaded in subclasses
    void dump( int offset = 0 );

    /// Print out the unit definition for all the unit tree
    void dumpUnitTree( int offset = 0 );

    /// Set a new property
    virtual void setProperty( std::string name, L0Muon::Property value );
    /// Set a new property
    virtual void setProperties( std::map<std::string, L0Muon::Property> properties );

    /// Retrieve a property
    L0Muon::Property getProperty( const std::string& name );

  protected:
    /*   std::string m_name; */

    Unit*                                    m_parent;
    std::map<std::string, L0Muon::Register*> m_inputs;     // input registers
    std::map<std::string, L0Muon::Register*> m_outputs;    // output registers
    std::vector<L0Muon::Unit*>               m_units;      // subunits
    std::map<std::string, L0Muon::Property>  m_properties; // properties

    bool m_debug;
  };

} // namespace L0Muon

#endif // PROCESSORKERNEL_UNIT_H
