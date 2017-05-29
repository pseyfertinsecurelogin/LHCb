#ifndef STAGINGSTREAMTOOL_H
#define STAGINGSTREAMTOOL_H 1

// Include files

// STL
#include <vector>

// boost
#include <boost/regex.hpp>

// Gaudi
#include <GaudiKernel/Service.h>
#include <GaudiKernel/IEvtSelector.h>
#include <GaudiKernel/DataStreamTool.h>
#include <GaudiKernel/AlgTool.h>

namespace Gaudi {
   class IFileCatalog;
}
class IFileStagerSvc;

/** @class StagedStreamTool StagedStreamTool.h
 *
 *
 *  @author Roel Aaij
 *
 *  @date   2010-06-06
 *
 */
class GAUDI_API StagedStreamTool: public DataStreamTool {
public:

   /// Standard constructor
   StagedStreamTool( const std::string& type,
                      const std::string& name,
                      const IInterface* parent );

   StatusCode initialize() override;

   StatusCode addStreams(const StreamSpecs &) override;

   StatusCode clear() override;

 private:

   enum Type {
      NONE,
      LFN,
      PFN,
      GUID,
      COLLECTION
   };

   class Descriptor final {
   public:
      Descriptor( Type type, const std::string& descriptor )
         : m_type( type ), m_descriptor( descriptor )
      {

      }

      enum Type type() const
      {
         return m_type;
      }

      const std::string& descriptor() const
      {
         return m_descriptor;
      }

   private:

      enum Type m_type;
      std::string m_descriptor;

   };

   /// Fake copy constructor (never implemented).
   StagedStreamTool(const StagedStreamTool&) = delete;
   /// Fake assignment operator (never implemented).
   StagedStreamTool& operator= (const StagedStreamTool&) = delete;

   // Services
   SmartIF<IFileStagerSvc>  m_stagerSvc;
   SmartIF<Gaudi::IFileCatalog>  m_catalog;

   // Properties
   std::string m_catalogSvc;

   // Data members
   boost::regex m_regex;

   // Helper methods
   Descriptor extractFilename( const std::string& descriptor );

};
#endif // STAGINGSTREAMTOOL_H
