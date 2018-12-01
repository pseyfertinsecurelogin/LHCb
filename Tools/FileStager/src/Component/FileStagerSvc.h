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
#ifndef FILESTAGER_H
#define FILESTAGER_H 1

#include <vector>
#include <string>
#include <memory>

// boost
#include <boost/filesystem.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/cstdint.hpp>

// Gaudi
#include <GaudiKernel/Service.h>
#include <GaudiUtils/IIODataManager.h>

// FileStager
#include <FileStager/IFileStagerSvc.h>

// local
#include "File.h"

/** @class FileStagerSvc FileStagerSvc.h
 *
 *
 *  @author Roel Aaij
 *  @date   2009-11-21
 */
class FileStagerSvc : public extends< Service, IFileStagerSvc > {
public:

   FileStagerSvc( const std::string& name, ISvcLocator* svcLoc );

   StatusCode initialize() override;

   StatusCode finalize() override;

   StatusCode getLocal( const std::string& filename, std::string& local ) override;

   StatusCode addFiles( const std::vector< std::string >& files ) override;

   StatusCode clearFiles() override;

private:

   class FileWrapper {
   public:

      FileWrapper( File* file )
         : m_file( file )
      {
      }

      FileWrapper( const FileWrapper& other ) = default;

      File* file() const
      {
         return m_file;
      }

      const std::string& original() const
      {
         return m_file->original();
      }


      FileWrapper& operator=( const FileWrapper& other ) = delete;

   private:

      File* m_file;

   };

   struct originalTag{ };
   struct listTag{ };

   typedef boost::multi_index_container<
      FileWrapper,
      boost::multi_index::indexed_by<
         boost::multi_index::hashed_unique<
            boost::multi_index::tag< originalTag >,
            boost::multi_index::const_mem_fun< FileWrapper, const std::string&,
                                               &FileWrapper::original >
            >,
         boost::multi_index::sequenced<
            boost::multi_index::tag< listTag >
            >
         >
      > fileContainer_t;

   typedef fileContainer_t::index< originalTag >::type filesByOriginal_t;
   typedef fileContainer_t::index< listTag >::type filesByPosition_t;

   // iterator shortcuts
   typedef filesByOriginal_t::iterator original_iterator;
   typedef filesByPosition_t::iterator position_iterator;

   // const_iterator shortcuts
   typedef filesByOriginal_t::const_iterator const_original_iterator;
   typedef filesByPosition_t::const_iterator const_position_iterator;

   // Mutexes
   boost::recursive_mutex m_fileMutex;

   // Containers
   mutable fileContainer_t m_files;

   // Data
   std::unique_ptr<boost::thread> m_thread;
   std::string m_stageStart;
   int m_garbagePID = 0;
   const_original_iterator m_stageIt;

   // Services
   // SmartIF< Gaudi::IIODataManager > m_dataManager;

   // Properties
   Gaudi::Property<std::string> m_tmpdir
   { this, "Tempdir", "", "The base of the temporary directory "
                          "where the files will be staged" };
   Gaudi::Property<size_t> m_stageNFiles
   { this, "StageNFiles", 2, "The number of files to stage" };
   Gaudi::Property<size_t> m_tries
   { this, "DiskspaceTries", 10, "The number of times to retry "
                    "whether there is sufficient diskspace" };
   Gaudi::Property<size_t> m_copyTries
   { this, "CopyTries", 5, "Retry copying if it fails." };
   boost::filesystem::path m_tempdir;
   bool m_initialized = false;
   Gaudi::Property<bool> m_retry
   { this, "RetryStaging", false, "Retry staging once it's failed." };
   Gaudi::Property<bool> m_stageLocalFiles
   { this, "StageLocalFiles", false, "Stage files beginning with file:." };
   // Gaudi::Property<std::string> m_dataManagerName
   // { this, "DataManagerName","Gaudi::StagedIODataManager/IODataManager" ,
   //                  "Name of the IODataManager to use for proper disconnection." };
   Gaudi::Property<std::string> m_garbageCommand
   { this, "GarbageCollectorCommand", "garbage.exe" ,
                    "Command for the garbage collector." };
   Gaudi::Property<bool> m_checkLocalGarbage
   { this, "CheckForLocalGarbageCollector", true,
                    "Check if the garbage collector command is in the local directory." };
   Gaudi::Property<bool> m_keepFiles
   { this, "KeepFiles", false, "Keep staged files" };

   // Helper Methods
   void stage();

   boost::uintmax_t diskspace() const;

   void restartStaging( const std::string& filename );

   void removeFile( const_original_iterator it );

   void removeFiles();

   void removePrevious( const_original_iterator it );

   StatusCode garbage();

   File* createFile( const std::string& filename );

   bool keepFiles() const { return m_keepFiles; }
   void keepFilesHandler( Property& property );

   bool checkJobID() const;
};
#endif // FILESTAGER_H
