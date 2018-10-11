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
#ifndef __EVENTPACKER_EVENT_GENERICBUFFER_H__
#define __EVENTPACKER_EVENT_GENERICBUFFER_H__

#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/ClassID.h"
#include <vector>

namespace LHCb
{

  /** @class GenericBuffer
   *
   *  Generic char buffer for storing data.
   *
   *  @author Wouter Hulsbergen
   *  @date   2015-10-01
   */

  static const CLID CLID_GenericBuffer = 1556 ;

  class GenericBuffer : public DataObject
  {

  public:

    /// type of data holder
    typedef std::vector<char> DataType ;

    /// non-const accessor
    DataType& data() { return m_data; }

    /// const accessor
    const DataType& data() const { return m_data; }

    /// Class ID
    static const CLID& classID() { return CLID_GenericBuffer ; }

    /// Class ID
    const CLID& clID() const override { return GenericBuffer::classID(); }

  private:

    std::vector<char> m_data ;

  } ;
}

#endif
