#include "GitEntityResolver.h"

#include <sstream>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include <boost/utility/string_ref.hpp>

DECLARE_COMPONENT( GitEntityResolver )

namespace {

  template<class RET, class FUNC, class ...ARGS>
  RET git_call(boost::string_ref name, boost::string_ref err_msg, boost::string_ref key,
               FUNC func, ARGS&&... args) {
    typename RET::element_type * tmp = nullptr;
    if ( UNLIKELY( func(&tmp, std::forward<ARGS>(args)...) ) )
      throw GaudiException(std::string(err_msg) + " " + std::string(key) + ": " + giterr_last()->message, std::string(name), StatusCode::FAILURE);
    return RET{ tmp };
  }

}

GitEntityResolver::Blob::Blob( const git_object_ptr& obj ) {
  auto blob = reinterpret_cast<const git_blob*>( obj.get() );
  m_size = git_blob_rawsize( blob );
  auto buff = xercesc::XMLPlatformUtils::fgMemoryManager->allocate( m_size );
  std::memcpy(buff, git_blob_rawcontent( blob ), m_size );
  m_buff = reinterpret_cast<const XMLByte*>( buff );
}

GitEntityResolver::Blob::~Blob() {
  auto tmp = static_cast<const void*>( adopt() );
  if ( tmp ) xercesc::XMLPlatformUtils::fgMemoryManager->deallocate( const_cast<void*>( tmp ) );
}


#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define DEBUG_MSG ON_DEBUG debug()
#define ON_VERBOSE if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
#define VERBOSE_MSG ON_VERBOSE verbose()

GitEntityResolver::GitEntityResolver( const std::string& type, const std::string& name, const IInterface* parent ):
  base_class(type, name, parent)
{
  // Initialize Git library
  git_libgit2_init();

  declareProperty( "PathToRepository", m_pathToRepository = "", "path to the git repository to get data from");
  m_pathToRepository.declareUpdateHandler([this](Property&) -> void {
    DEBUG_MSG << "opening Git repository '" << m_pathToRepository.value() << "'" << endmsg;
    m_repository = git_call<git_repository_ptr>( this->name(), "cannot open repository", m_pathToRepository.value(),
                                                 git_repository_open, m_pathToRepository.value().c_str() );
  });

  declareProperty( "Commit", m_commit = "HEAD", "commit id (or tag, or branch) of the version to use");
}

GitEntityResolver::~GitEntityResolver() {
  // Finalize Git library
  git_libgit2_shutdown();
}

StatusCode GitEntityResolver::initialize ( ) {
  StatusCode sc = AlgTool::initialize();
  if ( ! sc.isSuccess() ) return sc;

  DEBUG_MSG << "Initializing..." << endmsg;

  if ( ! m_repository ) {
    m_pathToRepository.useUpdateHandler();
    if ( UNLIKELY( ! m_repository ) )
      throw GaudiException( "invalid Git repository: '" + m_pathToRepository.value() + "'", name(), StatusCode::FAILURE );
  }

  // Initialize the Xerces-C++ XML subsystem
  try {

    xercesc::XMLPlatformUtils::Initialize();

    DEBUG_MSG << "Successfully initialized." << endmsg;

  } catch(const xercesc::XMLException& toCatch) {
    char *message = xercesc::XMLString::transcode(toCatch.getMessage());
    fatal() << "Error during Xerces-C Initialization." << endmsg;
    fatal() << "  Exception message:" << message << endmsg;
    xercesc::XMLString::release(&message);
    sc = StatusCode::FAILURE;  // allow return value optimization
    return sc;
  }

  return sc;
}

StatusCode GitEntityResolver::finalize ( ) {

  m_repository.reset();

  // Finalize the Xerces-C++ XML subsystem
  xercesc::XMLPlatformUtils::Terminate();

  return AlgTool::finalize();
}


const std::vector<std::string>& GitEntityResolver::protocols() const {
  /// Vector of supported protocols. (for IFileAccess)
  static const std::vector<std::string> s_protocols = { { "git" } };
  return s_protocols;
}

GitEntityResolver::open_result_t GitEntityResolver::open(const std::string &url) {
  auto obj = i_getData(url);
  auto blob = reinterpret_cast<const git_blob*>( obj.get() );
  std::string str{ reinterpret_cast<const char*>( git_blob_rawcontent( blob ) ), static_cast<std::size_t>( git_blob_rawsize( blob ) ) };
  return open_result_t( new std::istringstream( str ) );
}

git_object_ptr GitEntityResolver::i_getData( const std::string &url ) const {
  boost::string_ref path{url};
  if ( url.compare( 0, 4, "git:") == 0 ) {
    if ( url.compare( 4, 2, "//" ) == 0 )
      path = path.substr(6);
    else
      path = path.substr(4);
  }
  while ( path[0] == '/' ) path = path.substr(1);

  std::string rev = m_commit.value() + ":" + std::string(path);
  return git_call<git_object_ptr>(name(), "cannot resolve object", rev,
                                  git_revparse_single, m_repository.get(), rev.c_str());
}

xercesc::InputSource* GitEntityResolver::resolveEntity(const XMLCh *const, const XMLCh *const systemId) {

  // copy Xerces string into std::string
  std::string systemIdString;
  char *cString = xercesc::XMLString::transcode(systemId);
  if (cString) {
    systemIdString = cString;
    xercesc::XMLString::release(&cString);
  }

  DEBUG_MSG << "systemId = \"" << systemIdString << "\"" << endmsg;

  if ( systemIdString.compare( 0, 4, "git:" ) != 0 ) {
    // the string does not start with "git:", so I cannot handle it
    VERBOSE_MSG << "Not a Git URL" << endmsg;
    // tell XercesC to use the default action
    return nullptr;
  }

  Blob data{ i_getData( systemIdString ) };

  auto buff_size = data.size();  // must be done here because "adopt" set the size to 0
  return new xercesc::MemBufInputSource(data.adopt(), buff_size, systemId, true);
}

void GitEntityResolver::defaultTags ( std::vector<LHCb::CondDBNameTagPair>& tags ) const {
  tags.emplace_back( m_pathToRepository, m_commit );
}
