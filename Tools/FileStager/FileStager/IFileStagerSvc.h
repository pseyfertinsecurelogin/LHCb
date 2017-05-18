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
