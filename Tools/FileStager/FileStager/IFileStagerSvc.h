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
#ifndef IFILESTAGERSVC_H
#define IFILESTAGERSVC_H

// stdlib
#include <string>

// Gaudi
#include <GaudiKernel/IInterface.h>

// Forward declarations
class StageManager;


/** @class IFileStagerSvc IFileStagerSvc.h FileStager/IFileStagerSvc.h
 *
 *   The interface implemented by the FileStagerSvc service.
 *
 *   @author Daniela Remenska
 *   @version 1.0
 *
 */
struct GAUDI_API IFileStagerSvc : extend_interfaces<IInterface>
{
   /// InterfaceID
   DeclareInterfaceID( IFileStagerSvc, 2, 0 );

   virtual StatusCode getLocal( const std::string& filename, std::string& local ) = 0;

   virtual StatusCode addFiles( const std::vector< std::string >& files ) = 0;

   virtual StatusCode clearFiles() = 0;

};

#endif // IFILESTAGERSVC_H
