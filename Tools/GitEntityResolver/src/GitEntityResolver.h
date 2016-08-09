#ifndef GITENTITYRESOLVER_H
#define GITENTITYRESOLVER_H

// Include files
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IFileAccess.h"
#include "GaudiKernel/Time.h"
#include "GitEntityResolver/helpers.h"
#include "Kernel/ICondDBInfo.h"
#include "XmlTools/IXmlEntityResolver.h"
#include <boost/utility/string_ref.hpp>
#include <type_traits>

#include <iosfwd>

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
class GitEntityResolver : public extends<AlgTool, IXmlEntityResolver, IFileAccess, ICondDBInfo>,
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
  using open_result_t = typename std::result_of<decltype ( &IFileAccess::open )( IFileAccess&, std::string )>::type;
  open_result_t open( const std::string& url ) override;

  /// @see IFileAccess::protocols
  const std::vector<std::string>& protocols() const override;

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const override;

private:
  /// path to the git repository to get data from
  StringProperty m_pathToRepository;
  /// commit id (or tag, or branch) of the version to use
  StringProperty m_commit;
  /// name of the IDetDataSvc, used to get the current event time
  StringProperty m_detDataSvcName;

  /// internal flag used to track if we are using the Git database or checked out files
  bool m_useFiles = false;

  /// Helper class to propagate info about needed IOV.
  struct IOVInfo {
    std::string key;
    Gaudi::Time since;
    Gaudi::Time until;
  };
  friend std::ostream& operator<<( std::ostream& s, const IOVInfo& info );

  /// actual implementation of open method, depending on the use of git objects or files
  template <class T>
  open_result_t i_open( const T& obj, const std::string& url )
  {
    using Git::Helpers::is_dir;
    if ( is_dir( obj ) ) {
      return open( url + "/" + i_getIOVInfo( url ).key );
    } else {
      return i_makeIStream( obj );
    }
  }

  /// helper to convert to std::istream.
  template <class T>
  open_result_t i_makeIStream( const T& obj ) const;

  /// for a given URL, retrieve the payload key to use for the current event time.
  IOVInfo i_getIOVInfo( const std::string& url );

  /// Helper class to manage Xerces-C++ buffers
  struct Blob {
    Blob( const git_object_ptr& obj );
    Blob( open_result_t&& f );
    Blob( Blob&& other )
    {
      m_size = other.m_size;
      m_buff = other.adopt();
    }
    Blob( const Blob& ) = delete;

    ~Blob();
    const XMLByte* get() { return m_buff; }
    XMLSize_t size() { return m_size; }
    const XMLByte* adopt()
    {
      const XMLByte* tmp = m_buff;
      m_buff             = nullptr;
      m_size             = 0;
      return tmp;
    }

  private:
    const XMLByte* m_buff = nullptr;
    XMLSize_t m_size      = 0;
  };

  /// Return a string containing the data at a given path in the repository.
  git_object_ptr i_getData( boost::string_ref url ) const;

  /// helper to make a Xerces-C++ input source
  xercesc::InputSource* mkInputSource( GitEntityResolver::Blob data, const XMLCh* const systemId );

  git_repository_ptr m_repository;

  /// used to get the current event time
  SmartIF<IDetDataSvc> m_detDataSvc;
};

#endif // GITENTITYRESOLVER_H
