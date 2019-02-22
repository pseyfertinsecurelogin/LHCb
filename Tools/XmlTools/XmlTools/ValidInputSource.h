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
#ifndef VALIDINPUTSOURCE_H_
#define VALIDINPUTSOURCE_H_

#include <GaudiKernel/IValidity.h>
#include <GaudiKernel/Time.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XercesDefs.hpp>

#include <xercesc/framework/MemoryManager.hpp>

#include <xercesc/framework/MemBufInputSource.hpp>

/** @class ValidInputSource XmlTools/ValidInputSource.h
 *
 *  Extend xercesc::InputSource adding a validity (IValidity interface).
 *
 *  @author Marco Clemencic
 *  @date   2006-02-01
 */
class ValidInputSource : public xercesc::MemBufInputSource, virtual public IValidity {

public:
  /// Constructor
  ValidInputSource( const XMLByte* const srcDocBytes, const XMLSize_t byteCount, const XMLCh* const bufId,
                    const bool                    adoptBuffer = false,
                    xercesc::MemoryManager* const manager     = xercesc::XMLPlatformUtils::fgMemoryManager );

  /// Constructor
  ValidInputSource( std::string buffer, const XMLCh* const bufId,
                    xercesc::MemoryManager* const manager = xercesc::XMLPlatformUtils::fgMemoryManager );

  // -------- Implementation of IValidity --------

  /// is the Object valid? (it can be always invalid)
  bool isValid() const override;

  /// is the Object valid for a given time?
  bool isValid( const Gaudi::Time& time ) const override;

  /// since what time the Objest is valid?
  const Gaudi::Time& validSince() const override;

  /// till  what time the Object is Valid?
  const Gaudi::Time& validTill() const override;

  /// set the validity range of the Object
  void setValidity( const Gaudi::Time& since, const Gaudi::Time& until ) override;

  /// set the validity time of the Object
  void setValiditySince( const Gaudi::Time& since ) override;

  /// set the validity time of the Object
  void setValidityTill( const Gaudi::Time& until ) override;

private:
  /// Time of the start of validity
  Gaudi::Time m_since;

  /// Time of end of validity
  Gaudi::Time m_until;

  /// optional buffer, external to xercesc
  std::string m_buffer;
};

#endif /*VALIDINPUTSOURCE_H_*/
