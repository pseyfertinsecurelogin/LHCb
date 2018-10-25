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
#include "GitEntityResolver.h"

#include <fstream>
#include <sstream>

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "XmlTools/ValidInputSource.h"

DECLARE_COMPONENT( GitEntityResolver )

#include <boost/version.hpp>
#if BOOST_VERSION >= 106100
#include <boost/filesystem/path.hpp>
namespace
{
  std::string normalize( boost::filesystem::path path ) { return path.lexically_normal().generic_string(); }
}
#else
#include <regex>
namespace
{
  /// helper to normalize relative paths
  std::string normalize( std::string path )
  {
    // regex for entries to be removed, i.e. "/parent/../" and "/./"
    static const std::regex ignored_re{"(/[^/]+/\\.\\./)|(/\\./)"};
    std::string old_path;
    while ( old_path.length() != path.length() ) {
      old_path.swap( path );
      path = std::regex_replace( old_path, ignored_re, "/" );
    }
    return path;
  }
}
#endif

namespace
{

  template <class RET, class FUNC, class... ARGS>
  RET git_call( boost::string_ref name, boost::string_ref err_msg, boost::string_ref key, FUNC func, ARGS&&... args )
  {
    typename RET::element_type* tmp = nullptr;
    if ( UNLIKELY( func( &tmp, std::forward<ARGS>( args )... ) ) )
      throw GaudiException( std::string( err_msg ) + " " + std::string( key ) + ": " + giterr_last()->message,
                            std::string( name ), StatusCode::FAILURE );
    return RET{tmp};
  }

  boost::string_ref strip_prefix( boost::string_ref url )
  {
    if ( url.starts_with( "git:" ) ) url = url.substr( 4 ); // strip the "git:" prefix
    while ( url[0] == '/' ) url          = url.substr( 1 ); // strip optional leading '/'s
    return url;
  }

  /// Helper class to manage Xerces-C++ buffers
  struct Blob {
    Blob( GitEntityResolver::open_result_t&& f )
    {
      m_size    = f->seekg( 0, std::ifstream::end ).tellg();
      auto buff = xercesc::XMLPlatformUtils::fgMemoryManager->allocate( m_size );
      f->seekg( 0 ).read( reinterpret_cast<std::ifstream::char_type*>( buff ), m_size );
      m_buff = reinterpret_cast<const XMLByte*>( buff );
    }
    Blob( const Blob& ) = delete;

    ~Blob()
    {
      auto tmp = static_cast<const void*>( adopt() );
      if ( tmp ) xercesc::XMLPlatformUtils::fgMemoryManager->deallocate( const_cast<void*>( tmp ) );
    }

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

  /// helper to extract the file name from a full path
  boost::string_ref basename( boost::string_ref path )
  {
    // note: if '/' is not found, we get npos and npos + 1 is 0
    return path.substr( path.rfind( '/' ) + 1 );
  }

  // note: copied from DetCond/src/component/CondDBCommon.cpp
  std::string generateXMLCatalog( const std::string& root, const std::vector<std::string>& dirs,
                                  const std::vector<std::string>& files )
  {
    std::ostringstream xml; // buffer for the XML

    auto name = basename( root );
    // XML header, root element and catalog initial tag
    xml << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
        << "<!DOCTYPE DDDB SYSTEM \"git:/DTD/structure.dtd\">"
        << "<DDDB><catalog name=\"" << name << "\">";

    // sub-folders are considered as container of conditions
    for ( boost::string_ref f : files ) {
      f = basename( f );
      // Ignore folders with .xml or .txt extension.
      // We never used .xml for Online conditions and after the Hlt1/Hlt2 split
      // we need to avoid automatic mapping for the .xml files.
      if ( !( f.ends_with( ".xml" ) || f.ends_with( ".txt" ) ) ) {
        xml << "<conditionref href=\"" << name << '/' << f << "\"/>";
      }
    }
    // sub-foldersets are considered as catalogs
    for ( boost::string_ref f : dirs ) {
      f = basename( f );
      xml << "<catalogref href=\"" << name << '/' << f << "\"/>";
    }
    // catalog and root element final tag
    xml << "</catalog></DDDB>";

    return xml.str();
  }
}

std::ostream& operator<<( std::ostream& s, const GitEntityResolver::IOVInfo& info )
{
  return s << info.key << " [" << info.since << ", " << info.until << ")";
}

#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define DEBUG_MSG ON_DEBUG debug()
#define ON_VERBOSE if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
#define VERBOSE_MSG ON_VERBOSE verbose()

GitEntityResolver::GitEntityResolver( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ), m_repository{[this]() -> git_repository_ptr::storage_t {
      auto lvl = FSMState() < Gaudi::StateMachine::RUNNING ? MSG::INFO : MSG::DEBUG;
      if ( msgLevel( lvl ) )
        msgStream( lvl ) << "opening Git repository '" << m_pathToRepository.value() << "'" << endmsg;

      auto res =
          git_call<git_repository_ptr::storage_t>( this->name(), "cannot open repository", m_pathToRepository.value(),
                                                   git_repository_open, m_pathToRepository.value().c_str() );
      if ( UNLIKELY( !res ) )
        throw GaudiException( "invalid Git repository: '" + m_pathToRepository.value() + "'", this->name(),
                              StatusCode::FAILURE );
      return res;
    }}
{
  // Initialize Git library
  git_libgit2_init();

  m_pathToRepository.declareUpdateHandler( [this]( Property& ) -> void {
    // on update of the property we disconnect
    m_repository.reset();
  } );

  m_ignoreRegex.declareUpdateHandler( [this]( Property& ) -> void {
    if ( !m_ignoreRegex.value().empty() ) {
      DEBUG_MSG << "ignoring paths matching '" << m_ignoreRegex.value() << "'" << endmsg;
    };
    m_ignore.assign( m_ignoreRegex.value() );
  } );
}

GitEntityResolver::~GitEntityResolver()
{
  // Finalize Git library
  git_libgit2_shutdown();
}

StatusCode GitEntityResolver::initialize()
{
  StatusCode sc = base_class::initialize();
  if ( !sc.isSuccess() ) return sc;

  DEBUG_MSG << "Initializing..." << endmsg;

  // empty commit id means "<files>" in a non-bare repository or "HEAD" for a bare one
  if ( m_commit.empty() ) {
    if ( git_repository_is_bare( m_repository.get() ) ) {
      m_commit = "HEAD";
    } else {
      m_useFiles = true;
      info() << "using checked out files in " << m_pathToRepository.value() << endmsg;
    }
  }

  if ( !m_useFiles ) {
    auto obj = git_call<git_object_ptr>( name(), "cannot resolve commit", m_commit.value(), git_revparse_single,
                                         m_repository.get(), m_commit.value().c_str() );

    {
      while ( git_object_type( obj.get() ) == GIT_OBJ_TAG ) {
        obj = git_call<git_object_ptr>( name(), "cannot resolve tag", m_commit.value(), git_tag_target,
                                        (git_tag*)obj.get() );
      }

      char oid[GIT_OID_HEXSZ + 1] = {0};
      git_oid_fmt( oid, git_object_id( obj.get() ) );

      if ( LIKELY( msgLevel( MSG::INFO ) ) ) {
        auto& log = info();
        log << "using commit '" << m_commit.value() << "'";
        if ( m_commit.value().compare( oid ) != 0 ) log << " corresponding to " << oid;
        log << endmsg;
      }
      // truncate oid to 8 characters
      oid[8]       = 0;
      m_defaultTag = m_commit.value() + '[' + oid + ']';
    }
    if ( UNLIKELY( m_limitToLastCommitTime ) ) {
      // get the time of the requested commit/tag
      m_lastCommitTime = Gaudi::Time( git_commit_time( (git_commit*)obj.get() ), 0 );
      DEBUG_MSG << "limit validity to commit time: " << m_lastCommitTime.format( false, "%Y-%m-%d %H:%M:%S" ) << "."
                << m_lastCommitTime.nanoformat() << " UTC" << endmsg;
    }
  } else {
    m_defaultTag = "<files>";
  }

  m_incSvc = service<IIncidentSvc>( "IncidentSvc" );
  if ( m_incSvc ) {
    DEBUG_MSG << "registering to IncidentSvc" << endmsg;
    // we use a very low priority for BeginEvent to be triggered after UpdateManagerSvc
    m_incSvc->addListener( this, IncidentType::BeginEvent, -100 );
    // we use a very high priority for special incident APP_INITIALIZED to be
    // called before fork/checkpoint in Online environment
    m_incSvc->addListener( this, "APP_INITIALIZED", 100 );
  } else {
    warning() << "cannot get IncidentSvc, automatic disconnect not possible" << endmsg;
  }

  // Initialize the Xerces-C++ XML subsystem
  try {

    xercesc::XMLPlatformUtils::Initialize();

    DEBUG_MSG << "Successfully initialized." << endmsg;

  } catch ( const xercesc::XMLException& toCatch ) {
    char* message = xercesc::XMLString::transcode( toCatch.getMessage() );
    fatal() << "Error during Xerces-C Initialization." << endmsg;
    fatal() << "  Exception message:" << message << endmsg;
    xercesc::XMLString::release( &message );
    sc = StatusCode::FAILURE; // allow return value optimization
    return sc;
  }

  return sc;
}

StatusCode GitEntityResolver::finalize()
{

  if ( m_incSvc ) {
    DEBUG_MSG << "deregistering from IncidentSvc" << endmsg;
    m_incSvc->removeListener( this );
  }

  m_repository.reset();
  m_detDataSvc.reset();

  // Finalize the Xerces-C++ XML subsystem
  xercesc::XMLPlatformUtils::Terminate();

  return base_class::finalize();
}

void GitEntityResolver::handle( const Incident& )
{
  // disconnect from the repository
  m_repository.reset();
}

const std::vector<std::string>& GitEntityResolver::protocols() const
{
  /// Vector of supported protocols. (for IFileAccess)
  static const std::vector<std::string> s_protocols = {{"git"}};
  return s_protocols;
}

git_object_ptr GitEntityResolver::i_getData( boost::string_ref path ) const
{
  std::string rev = m_commit.value() + ":" + normalize( path.to_string() );
  return git_call<git_object_ptr>( name(), "cannot resolve object", rev, git_revparse_single, m_repository.get(),
                                   rev.c_str() );
}

GitEntityResolver::IOVInfo GitEntityResolver::i_getIOVInfo( const std::string& url )
{
  if ( UNLIKELY( !m_detDataSvc ) ) {
    m_detDataSvc = service<IDetDataSvc>( m_detDataSvcName );
  }
  const std::string iovs_url = url + "/IOVs";
  if ( i_exists( iovs_url ) ) {
    auto data              = i_open( iovs_url ).first;
    const auto when        = m_detDataSvc->eventTime();
    std::int_fast64_t time = when.ns();
    DEBUG_MSG << "getting payload key for " << url << " at " << time << endmsg;

    std::string line;
    std::int_fast64_t current = 0, since = 0, until = m_lastCommitTime.ns();
    std::string key;

    if ( UNLIKELY( time >= until && FSMState() == Gaudi::StateMachine::RUNNING ) ) {
      std::stringstream msg;
      // this message is not appropriate, but matches the message used in CondDBAccessSvc
      msg << "Database not up-to-date. Latest known update is at "
          << m_lastCommitTime.format( false, "%Y-%m-%d %H:%M:%S" ) << "." << m_lastCommitTime.nanoformat()
          << " UTC, event time is " << when.format( false, "%Y-%m-%d %H:%M:%S" ) << "." << when.nanoformat() << " UTC";
      throw GaudiException( msg.str(), name(), StatusCode::FAILURE );
    }

    while ( std::getline( *data, line ) ) {
      std::istringstream is{line};
      is >> current;
      if ( current > time ) {
        until = current; // what we read is the "until" for the previous key
        break;           // and we need to use the previous key
      }
      is >> key;
      since = current; // the time we read is the "since" for the read key
    }
    IOVInfo info{key, since, until};
    DEBUG_MSG << "got " << info << endmsg;
    return info;
  } else {
    return {};
  }
}

template <>
GitEntityResolver::open_result_t GitEntityResolver::i_makeIStream<std::string>( const std::string& path ) const
{
  return open_result_t( new std::ifstream{path} );
}

template <>
GitEntityResolver::open_result_t GitEntityResolver::i_makeIStream<git_object_ptr>( const git_object_ptr& obj ) const
{
  auto blob = reinterpret_cast<const git_blob*>( obj.get() );
  std::string str{reinterpret_cast<const char*>( git_blob_rawcontent( blob ) ),
                  static_cast<std::size_t>( git_blob_rawsize( blob ) )};
  return open_result_t( new std::istringstream( str ) );
}

template <>
GitEntityResolver::open_result_t
GitEntityResolver::i_makeIStream<GitEntityResolver::dir_content>( const GitEntityResolver::dir_content& dirlist ) const
{
  return open_result_t( new std::istringstream( generateXMLCatalog( dirlist.root, dirlist.dirs, dirlist.files ) ) );
}

std::pair<GitEntityResolver::open_result_t, GitEntityResolver::IOVInfo>
GitEntityResolver::i_open( const std::string& url )
{
  DEBUG_MSG << "open(\"" << url << "\")" << ( m_useFiles ? " [files]" : "" ) << endmsg;
  auto path = strip_prefix( url );
  if ( UNLIKELY( std::regex_match( path.begin(), path.end(), m_ignore ) ) ) {
    VERBOSE_MSG << "path ignored" << endmsg;
    return {};
  }
  return UNLIKELY( m_useFiles ) ? i_open( m_pathToRepository.value() + "/" + path.to_string(), url )
                                : i_open( i_getData( path ), url );
}

bool GitEntityResolver::i_exists( const std::string& url ) const
{
  bool result = true;
  auto path   = strip_prefix( url );
  if ( UNLIKELY( m_useFiles ) )
    result = boost::filesystem::exists( m_pathToRepository.value() + "/" + path.to_string() );
  else {
    git_object* tmp = nullptr;
    git_revparse_single( &tmp, m_repository.get(), ( m_commit.value() + ":" + normalize( path.to_string() ) ).c_str() );
    result = tmp;
    git_object_free( tmp );
  };
  return result;
}

xercesc::InputSource* GitEntityResolver::resolveEntity( const XMLCh* const, const XMLCh* const systemId )
{

  // copy Xerces string into std::string
  std::string systemIdString;
  char* cString = xercesc::XMLString::transcode( systemId );
  if ( cString ) {
    systemIdString = cString;
    xercesc::XMLString::release( &cString );
  }

  DEBUG_MSG << "resolveEntity(systemId = \"" << systemIdString << "\")" << endmsg;

  boost::string_ref url = systemIdString;

  if ( !url.starts_with( "git:" ) ) {
    // the string does not start with "git:", so I cannot handle it
    VERBOSE_MSG << "not a Git URL" << endmsg;
    // tell XercesC to use the default action
    return nullptr;
  }

  auto data = i_open( url.to_string() );
  if ( UNLIKELY( !data.first ) ) return nullptr;

  Blob blob{std::move( data.first )};
  const auto buff_size = blob.size(); // must be done here because "adopt" set the size to 0

  std::unique_ptr<ValidInputSource> src{new ValidInputSource{blob.adopt(), buff_size, systemId, true}};
  src->setValidity( data.second.since, data.second.until );
  return src.release();
}

void GitEntityResolver::defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags ) const
{
  tags.emplace_back( m_pathToRepository, m_defaultTag );
}

GitEntityResolver::dir_content GitEntityResolver::i_listdir( const std::string& path, const std::string& url ) const
{
  using boost::filesystem::directory_iterator;
  using boost::filesystem::directory_entry;
  using boost::filesystem::is_directory;
  using boost::filesystem::exists;
  dir_content entries;
  entries.root = url;
  std::for_each( directory_iterator( path ), directory_iterator(), [&entries]( const directory_entry& d ) {
    ( ( is_directory( d.status() ) && !exists( d.path() / "IOVs" ) ) ? entries.dirs : entries.files )
        .emplace_back( d.path().generic_string() );
  } );
  return entries;
}

GitEntityResolver::dir_content GitEntityResolver::i_listdir( const git_object_ptr& obj, const std::string& url ) const
{
  dir_content entries;
  entries.root             = url;
  const git_tree* tree     = reinterpret_cast<const git_tree*>( obj.get() );
  std::size_t max_i        = git_tree_entrycount( tree );
  const git_tree_entry* te = nullptr;
  std::string te_url;

  for ( std::size_t i = 0; i < max_i; ++i ) {
    te     = git_tree_entry_byindex( tree, i );
    te_url = url + "/" + git_tree_entry_name( te );
    //
    ( ( Git::Helpers::is_dir( te ) && !i_exists( te_url + "/IOVs" ) ) ? entries.dirs : entries.files )
        .emplace_back( std::move( te_url ) );
  }
  return entries;
}
