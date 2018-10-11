/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//----------------------------------------------------------------------------
/** @file DeRichBase.h
 *
 *  Header file for detector description class : DeRichBase
 *
 *  @author Chris Jones    jonesc@hep.phy.cam.ac.uk
 *  @date   26-07-2009
 */
//----------------------------------------------------------------------------

#pragma once

// STL
#include <memory>

// Gaudi
#include "GaudiKernel/MsgStream.h"

// DetDesc
#include "DetDesc/DetectorElement.h"

// RichDet
#include "RichDet/DeRichLocations.h"

// Utils
#include "RichUtils/RichSIMDTypes.h"

// Vc
#include <Vc/common/alignedbase.h>

class DeRichSystem;

// Some defines for debug/verbose messages...
#ifndef _ri_debug
#define _ri_debug                                                                                  \
  if ( msgLevel( MSG::DEBUG ) ) debug()
#endif
#ifndef _ri_verbo
#define _ri_verbo                                                                                  \
  if ( msgLevel( MSG::VERBOSE ) ) verbose()
#endif

/** @class DeRichBase RichDet/DeRichBase.h
 *
 *  Base class for all Rich DetectorElements
 *
 *  @author Chris Jones
 *  @date   2009-07-26
 */
class DeRichBase : public DetectorElement, public Vc::AlignedBase< Vc::VectorAlignment >
{

public:

  /// Standard constructor
  DeRichBase( const std::string &name = "" ) : DetectorElement( name ) { resetMsgStream(); }

  /// Destructor
  virtual ~DeRichBase() = default;

protected:

  /// Set the name for this detector element
  inline void setMyName( const std::string &name )
  {
    m_myname = name;
    // force a new MsgStream object using this new name
    resetMsgStream();
  }

  /** Returns the name of this particular Detector Element
   *  @return name
   */
  inline const std::string &myName() const { return ( m_myname.empty() ? name() : m_myname ); }

protected:

  /** Create a MsgStream object on the fly, when first needed
   *
   *  @return Reference to the predefined stream
   */
  inline MsgStream &msgStream() const noexcept { return *m_msgStream.get(); }

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
  inline MsgStream &msgStream( const MSG::Level level ) const { return msgStream() << level; }

  /// shortcut for the method msgStream ( MSG::ALWAYS )
  inline MsgStream &always() const { return msgStream( MSG::ALWAYS ); }
  /// shortcut for the method msgStream ( MSG::FATAL   )
  inline MsgStream &fatal() const { return msgStream( MSG::FATAL ); }
  /// shortcut for the method msgStream ( MSG::ERROR   )
  inline MsgStream &error() const { return msgStream( MSG::ERROR ); }
  /// shortcut for the method msgStream ( MSG::WARNING )
  inline MsgStream &warning() const { return msgStream( MSG::WARNING ); }
  /// shortcut for the method msgStream ( MSG::INFO    )
  inline MsgStream &info() const { return msgStream( MSG::INFO ); }
  /// shortcut for the method msgStream ( MSG::DEBUG   )
  inline MsgStream &debug() const { return msgStream( MSG::DEBUG ); }
  /// shortcut for the method msgStream ( MSG::VERBOSE )
  inline MsgStream &verbose() const { return msgStream( MSG::VERBOSE ); }

  /// Temporary Message Stream object with a custom name
  MsgStream msgStream( const std::string &name ) const { return MsgStream( msgSvc(), name ); }

  /// Return the current message level
  inline MSG::Level msgLevel() const { return msgStream().level(); }

  /// Test the message level for the cached MsgStream object
  inline bool msgLevel( const MSG::Level level ) const { return UNLIKELY( msgLevel() <= level ); }

  /// Test the message level for a given MsgStream object
  inline bool msgLevel( const MSG::Level level, MsgStream &msgStream ) const
  {
    return UNLIKELY( msgStream.level() <= level );
  }

  /// Report a fatal
  inline StatusCode Fatal( const std::string &mess,
                           const StatusCode   sc = StatusCode::FAILURE ) const
  {
    fatal() << mess << endmsg;
    return sc;
  }

  /// Report an error
  inline StatusCode Error( const std::string &mess,
                           const StatusCode   sc = StatusCode::FAILURE ) const
  {
    error() << mess << endmsg;
    return sc;
  }

  /// Report a warning
  inline StatusCode Warning( const std::string &mess,
                             const StatusCode   sc = StatusCode::FAILURE ) const
  {
    warning() << mess << endmsg;
    return sc;
  }

private:

  /// reset the message stream with the currently set name
  void resetMsgStream() { m_msgStream = std::make_unique< MsgStream >( msgSvc(), myName() ); }

protected:

  /// Access DeRichSystem on demand
  DeRichSystem *deRichSys();

private:

  std::string                  m_myname = "";       ///< The name of this detector element
  std::unique_ptr< MsgStream > m_msgStream;         ///< Message Stream Object
  DeRichSystem *               m_deRichS = nullptr; ///< Pointer to the overall RICH system object
};
