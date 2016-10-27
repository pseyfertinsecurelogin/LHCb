#ifndef IOVDOMDOCUMENT_
#define IOVDOMDOCUMENT_

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

  /// Destructor
  virtual ~IOVDOMDocument();

  /// Return the contained DOMDocument.
  inline xercesc::DOMDocument * getDOM() const { return m_dom; }

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
  Gaudi::Time m_since;

  /// Time of end of validity
  Gaudi::Time m_until;

  /// The contained document
  xercesc::DOMDocument *m_dom;
};


#endif /*IOVDOMDOCUMENT_*/
