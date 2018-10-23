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
#ifndef EVENT_PACKERUTILS_H 
#define EVENT_PACKERUTILS_H 1
#include <string>
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/DataObject.h"


namespace LHCb
{
  namespace Packer
  {
    namespace Utils
    {

      /** Returns the full location of the given object in the Data Store
       *
       *  @param pObj Data object
       *
       *  @return Location of given data object
       */
      std::string location( const DataObject * pObj )
      {
        return ( !pObj ? "Null DataObject !" :
                 (pObj->registry() ? pObj->registry()->identifier() : "UnRegistered") );
      }

    }
  }
}     

#endif // EVENT_PACKERUTILS_H
