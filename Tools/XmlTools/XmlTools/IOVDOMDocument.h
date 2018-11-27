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
#ifndef IOVDOMDOCUMENT_
#define IOVDOMDOCUMENT_
#include <memory>
#include "GaudiKernel/Time.h"
#include "GaudiKernel/IValidity.h"

#include <xercesc/dom/DOMDocument.hpp>


/** @class IOVDOMDocument XmlTools/IOVDOMDocument.h
 *
 *  Small class to hold a DOMDocument together with a validity.
 *
 *  @author Marco Clemencic
 *  @date   2006-02-01
 */

class IOVDOMDocument: virtual public IValidity {

public:

  /// Constructor
  IOVDOMDocument(xercesc::DOMDocument *dom);


  /// Return the contained DOMDocument.
  inline xercesc::DOMDocument * getDOM() const { return m_dom.get(); }

  // -------- Implementation of IValidity --------

  /// is the Object valid? (it can be always invalid)
  bool isValid() const override;

  /// is the Object valid for a given time?
  bool isValid( const Gaudi::Time& time) const override;

  /// since what time the Objest is valid?
  const Gaudi::Time& validSince() const override;

  /// till  what time the Object is Valid?
  const Gaudi::Time& validTill() const override;

  /// set the validity range of the Object
  void setValidity( const Gaudi::Time& since, const Gaudi::Time& until) override;

  /// set the validity time of the Object
  void setValiditySince( const Gaudi::Time& since) override;

  /// set the validity time of the Object
  void setValidityTill( const Gaudi::Time& until) override;

private:

  /// Time of the start of validity
  Gaudi::Time m_since = Gaudi::Time::epoch();

  /// Time of end of validity
  Gaudi::Time m_until = Gaudi::Time::max();

  /// The contained document
  struct release {
    template <typename T> void operator()(T* p) const { if (p) p->release(); }
  };
  std::unique_ptr<xercesc::DOMDocument,release> m_dom;
};

#endif /*IOVDOMDOCUMENT_*/
