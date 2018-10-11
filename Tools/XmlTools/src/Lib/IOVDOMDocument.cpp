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
#include "XmlTools/IOVDOMDocument.h"

IOVDOMDocument::IOVDOMDocument(xercesc::DOMDocument *dom)
: m_dom(dom)
{}

bool IOVDOMDocument::isValid() const
{
  return validSince() <= validTill();
}

bool IOVDOMDocument::isValid( const Gaudi::Time& time ) const
{
  return validSince() <= time &&  time < validTill();
}

const Gaudi::Time& IOVDOMDocument::validSince() const
{
  return m_since;
}

const Gaudi::Time& IOVDOMDocument::validTill() const
{
  return m_until;
}

void IOVDOMDocument::setValidity( const Gaudi::Time& since, const Gaudi::Time& until)
{
  m_since = since;
  m_until = until;
}

void IOVDOMDocument::setValiditySince( const Gaudi::Time& since)
{
  m_since = since;
}

void IOVDOMDocument::setValidityTill( const Gaudi::Time& until)
{
  m_until = until;
}
