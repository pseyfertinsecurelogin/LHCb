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
#ifndef GITENTITYRESOLVER_H
#define GITENTITYRESOLVER_H

// Include files
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IFileAccess.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Time.h"
#include "GitEntityResolver/helpers.h"
#include "Kernel/ICondDBInfo.h"
#include "XmlTools/IXmlEntityResolver.h"

#include <algorithm>
#include <iosfwd>
#include <regex>
#include <type_traits>

#include <boost/utility/string_ref.hpp>

// this should be defined in GaudiKernel/Time.h, but the version there is not
// found by ADL
namespace Gaudi
{
  using ::operator<;
}

/** Allow use of a Git repository as a source of XML files for XercesC.
 *
 *  An instance of this tool can be used to allow inclusion of XML DTDs and documents
 *  from a Git repository. It works implementing the EntityResolver interface defined in Xerces-C++,
 *  which allows to intercept the requests of external XML data.
 *
 *  Examples of CondDB external references:
 *  \li \<!DOCTYPE DDDB SYSTEM "git:/DTD/structure"\>
 *  \li \<!ENTITY Parameters SYSTEM "git:/Parameters/SpecialParamters"\>
 *
 *  This tool implements the Gaudi IFileAccess interface, so that it can be used
 *  to retrieve files from a Git repository.
 *
 *  @author Marco Clemencic
 *  @date   2016-07-21
 */
class GitEntityResolver : public extends<AlgTool, IXmlEntityResolver, IFileAccess, ICondDBInfo, IIncidentListener>,
                          virtual public xercesc::EntityResolver
{
public:
  GitEntityResolver( const std::string& type, const std::string& name, const IInterface* parent );

  ~GitEntityResolver() override;

  /// Initialize the tool
  StatusCode initialize() override;

  /// Finalize the tool
  StatusCode finalize() override;

  /// Return a pointer to the actual implementation of a xercesc::EntityResolver.
  xercesc::EntityResolver* resolver() override { return this; }

  /// Create a Xerces-C input source based on the given systemId (publicId is ignored).
  /// If the systemId does not begin with "conddb:", it returns NULL, so the parser go on with the default action.
  xercesc::InputSource* resolveEntity( const XMLCh* const publicId, const XMLCh* const systemId ) override;

  /// Find the URL and returns an auto_ptr to an input stream interface of an
  /// object that can be used to read from the file the URL is pointing to.
  /// Returns an empty pointer if the URL cannot be resolved.
  using open_result_t = std::unique_ptr<std::istream>;
  open_result_t open( const std::string& url ) override { return i_open( url ).first; }

  /// @see IFileAccess::protocols
  const std::vector<std::string>& protocols() const override;

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const override;

  /// Inform that a new incident has occurred
  void handle( const Incident& ) override;

private:
  Gaudi::Property<std::string> m_pathToRepository{this, "PathToRepository", "",
                                                  "path to the git repository to get data from"};
  Gaudi::Property<std::string> m_commit{this, "Commit", "HEAD", "commit id (or tag, or branch) of the version to use, "
                                                                "empty means use local files"};
  Gaudi::Property<std::string> m_detDataSvcName{this, "DetDataSvc", "DetectorDataSvc",
                                                "name of the IDetDataSvc, used to get the current event time"};
  Gaudi::Property<std::string> m_ignoreRegex{this, "Ignore", "",
                                             "regular expression matching paths that should be ignored"};
  Gaudi::Property<bool> m_limitToLastCommitTime{this, "LimitToLastCommitTime", false,
                                                "force upper limit of IOVs is last commit time"};

  /// internal flag used to track if we are using the Git database or checked out files
  bool m_useFiles = false;

  /// Helper class to propagate info about needed IOV.
  struct IOVInfo {
    IOVInfo() = default;
    IOVInfo( std::string k, const Gaudi::Time& s, const Gaudi::Time& u ) : key( std::move( k ) ), since( s ), until( u )
    {
    }
    std::string key   = "";
    Gaudi::Time since = Gaudi::Time::epoch();
    Gaudi::Time until = Gaudi::Time::max();
    void cut( const IOVInfo& boundary )
    {
      since = std::max( since, boundary.since );
      until = std::min( until, boundary.until );
    }
  };
  friend std::ostream& operator<<( std::ostream& s, const IOVInfo& info );

  /// actual implementation of open method, depending on the use of git objects or files
  template <class T>
  std::pair<open_result_t, IOVInfo> i_open( const T& obj, const std::string& url )
  {
    using Git::Helpers::is_dir;
    if ( is_dir( obj ) ) {
      auto info = i_getIOVInfo( url );
      if ( !info.key.empty() ) {
        auto result = i_open( url + "/" + info.key );
        result.second.cut( info );
        return result;
      } else {
        return { i_makeIStream( i_listdir( obj, url ) ), IOVInfo{} };
      }
    } else {
      return { i_makeIStream( obj ), IOVInfo{} };
    }
  }

  /// entry point to the implementation of the open method.
  std::pair<open_result_t, IOVInfo> i_open( const std::string& url );

  /// helper to convert to std::istream.
  template <class T>
  open_result_t i_makeIStream( const T& obj ) const;

  /// for a given URL, retrieve the payload key to use for the current event time.
  IOVInfo i_getIOVInfo( const std::string& url );

  /// Return the object containing the data at a given path in the repository.
  git_object_ptr i_getData( boost::string_ref url ) const;

  struct dir_content {
    std::string root;
    std::vector<std::string> dirs;
    std::vector<std::string> files;
  };
  /// helper to get the list of entries in a directory divided in directories and files
  dir_content i_listdir( const std::string& path, const std::string& url ) const;
  /// helper to get the list of entries in a directory divided in directories and files
  dir_content i_listdir( const git_object_ptr& obj, const std::string& url ) const;

  /// check if a url exists in the current repository
  bool i_exists( const std::string& url ) const;

  git_repository_ptr m_repository;

  /// used to get the current event time
  SmartIF<IDetDataSvc> m_detDataSvc;

  SmartIF<IIncidentSvc> m_incSvc;

  std::regex m_ignore;

  Gaudi::Time m_lastCommitTime = Gaudi::Time::max();

  /// cache for the string to be reported by the defaultTags() method.
  std::string m_defaultTag;
};

#endif // GITENTITYRESOLVER_H
