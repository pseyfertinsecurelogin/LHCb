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
// Include files
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/Time.h"

// local
#include "CondDBDispatcherSvc.h"
#include "CondDBCommon.h"

// Factory implementation
DECLARE_COMPONENT( CondDBDispatcherSvc )

//-----------------------------------------------------------------------------
// Implementation file for class : CondDBDispatcherSvc
//
// 2006-07-10 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CondDBDispatcherSvc::CondDBDispatcherSvc( const std::string& name, ISvcLocator* svcloc ):
  base_class(name,svcloc)
{
  declareProperty("MainAccessSvc", m_mainAccessSvcName = "CondDBAccessSvc" );
  declareProperty("Alternatives",  m_alternativesDeclarationMap            );

  declareProperty("EnableXMLDirectMapping", m_xmlDirectMapping = true,
                  "Allow direct mapping from CondDB structure to"
                  " transient store.");
}

//=============================================================================
// initialize
//=============================================================================
StatusCode CondDBDispatcherSvc::initialize(){
  StatusCode sc = base_class::initialize();
  if (sc.isFailure()) return sc;

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "Initialize" << endmsg;

  // locate the main access service
  m_mainDB = service(m_mainAccessSvcName,true);
  if (  !m_mainDB ) {
    error() << "Could not locate " << m_mainAccessSvcName << endmsg;
    return StatusCode::FAILURE;
  }

  // locate all the alternative AccessSvcs
  for (const auto& decl : m_alternativesDeclarationMap) {
    const std::string &altPath = decl.first;
    const std::string &svcName = decl.second;

    if ( m_alternatives.find(altPath) != m_alternatives.end() ) {
      error() << "More than one alternative for path " << altPath << endmsg;
      return StatusCode::FAILURE;
    }

    auto svcPtr = service<ICondDBReader>(svcName,true);
    if (  !svcPtr ) {
      error() << "Could not locate " << svcName << endmsg;
      return StatusCode::FAILURE;
    }

    m_alternatives[altPath] = std::move(svcPtr);
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Retrieved '" << svcName << "' (for path '" << altPath << "')" << endmsg;

  }

  return sc;
}

//=============================================================================
// finalize
//=============================================================================
StatusCode CondDBDispatcherSvc::finalize(){
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "Finalize" << endmsg;

  m_mainDB.reset();
  m_alternatives.clear();

  return base_class::finalize();
}

ICondDBReader::IOVList CondDBDispatcherSvc::getIOVs(const std::string & path, const IOV &iov, cool::ChannelId channel)
{
  return alternativeFor(path)->getIOVs(path, iov, channel);
}

ICondDBReader::IOVList CondDBDispatcherSvc::getIOVs(const std::string & path, const IOV &iov, const std::string & channel)
{
  return alternativeFor(path)->getIOVs(path, iov, channel);
}

//=========================================================================
//  find the appropriate alternative
//=========================================================================
ICondDBReader *CondDBDispatcherSvc::alternativeFor(const std::string &path) const {
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose() << "Get alternative DB for '" << path << "'" << endmsg;
  if ( path.empty() || (path == "/") ) {
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose() << "Root node: using '" << m_mainAccessSvcName << "'" << endmsg;
    return m_mainDB.get();
  }

  // loop over alternatives
  for (auto alt = m_alternatives.rbegin(); alt != m_alternatives.rend(); ++alt ) {
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) ) {
      verbose() << "Comparing with " << alt->first << endmsg;
    }
    // FIXME: (MCl) wrong logic
    //     path=/Conditions/Velo/AlignmentCatalog.xml
    //     alt.=/Conditions/Velo/Alignment
    //     Should not match
    if ( ( path.size() >= alt->first.size() ) &&
         ( path.substr(0,alt->first.size()) == alt->first ) ){
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) ) {
        auto svc = alt->second.as<IService>();
        verbose() << "Using '" << ( svc ? svc->name() : "unknown" ) << "'" << endmsg;
      }
      return alt->second;
    }
  }

  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose() << "Not found: using '" << m_mainAccessSvcName << "'" << endmsg;
  return m_mainDB.get();
}

//=========================================================================
//  retrieve an object
//=========================================================================
StatusCode CondDBDispatcherSvc::getObject (const std::string &path, const Gaudi::Time &when,
                                           DataPtr &data,
                                           std::string &descr, Gaudi::Time &since, Gaudi::Time &until, cool::ChannelId channel) {
  StatusCode sc;
  if (m_xmlDirectMapping && isFolderSet(path)) {
    descr = "Catalog generated automatically by " + name();
    since = Gaudi::Time::epoch();
    until = Gaudi::Time::max();
    sc = CondDB::generateXMLCatalog(this,path,data);
  } else {
    sc = alternativeFor(path)->getObject(path,when,data,descr,since,until,channel);
  }
  return sc;
}
StatusCode CondDBDispatcherSvc::getObject (const std::string &path, const Gaudi::Time &when,
                                           DataPtr &data,
                                           std::string &descr, Gaudi::Time &since, Gaudi::Time &until, const std::string &channel) {
  StatusCode sc;
  if (m_xmlDirectMapping && isFolderSet(path)) {
    descr = "Catalog generated automatically by " + name();
    since = Gaudi::Time::epoch();
    until = Gaudi::Time::max();
    sc = CondDB::generateXMLCatalog(this,path,data);
  } else {
    sc = alternativeFor(path)->getObject(path,when,data,descr,since,until,channel);
  }
  return sc;
}

//=========================================================================
//  get the list of child nodes of a folderset
//=========================================================================
StatusCode CondDBDispatcherSvc::getChildNodes (const std::string &path, std::vector<std::string> &node_names) {
  return getChildNodes(path,node_names,node_names);
}

//=========================================================================
//  get the list of child nodes of a folderset
//=========================================================================
StatusCode CondDBDispatcherSvc::getChildNodes (const std::string &path,
                                               std::vector<std::string> &folders,
                                               std::vector<std::string> &foldersets) {
  // clear the destination vectors
  folders.clear();
  foldersets.clear();

  // Get the folders and foldersets from the dedicated alternative
  std::vector<std::string> tmpv1,tmpv2;
  StatusCode sc = alternativeFor(path)->getChildNodes(path,tmpv1,tmpv2);
  if (sc.isFailure()) return sc;

  // Find alternatives for subfolders of the path.
  auto path_size = path.size();
  for ( auto alt = m_alternatives.rbegin(); alt != m_alternatives.rend(); ++alt ) {
    // check if the path for the alternative is a subfolder of the required path
    // i.e. alt->first should be = path + '/' + extra
    if ( ( alt->first.size() > (path_size+1) ) && // it must be long enough
         ( alt->first[path_size] == '/' ) &&
         ( alt->first.substr(0,path_size) == path ) ){
      // take the name of the child folder[set] implied by the alternative
      // i.e. substring from after (path+'/') to the next '/'
      std::string sub = alt->first.substr(path_size+1,
                                          alt->first.find('/',path_size+1)-(path_size+1));
      if (std::find(tmpv1.begin(),tmpv1.end(),sub) == tmpv1.end() &&
          std::find(tmpv2.begin(),tmpv2.end(),sub) == tmpv2.end()){
        // this subnode is an addition due to the alternative
        // let's check the type
        if (alt->second->isFolder(path+'/'+sub))
          tmpv1.push_back(sub); // folder
        else
          tmpv2.push_back(sub); // folderset
      }
    }
  }

  // copy the temporary vectors to the output ones
  folders = tmpv1;
  foldersets = tmpv2;
  return sc;
}

//=========================================================================
// Tells if the path is available in the database.
//=========================================================================
bool CondDBDispatcherSvc::exists(const std::string &path) {
  return alternativeFor(path)->exists(path);
}

//=========================================================================
// Tells if the path (if it exists) is a folder.
//=========================================================================
bool CondDBDispatcherSvc::isFolder(const std::string &path) {
  return alternativeFor(path)->isFolder(path);
}

//=========================================================================
// Tells if the path (if it exists) is a folderset.
//=========================================================================
bool CondDBDispatcherSvc::isFolderSet(const std::string &path) {
  return alternativeFor(path)->isFolderSet(path);
}

//=========================================================================
// Force disconnection from database.
//=========================================================================
void CondDBDispatcherSvc::disconnect() {
  // loop over alternatives
  for (auto&  alt : m_alternatives) alt.second->disconnect();
  if (m_mainDB) m_mainDB->disconnect();
}

//=========================================================================
// Collect the list of used tags and databases
//=========================================================================
void CondDBDispatcherSvc::defaultTags ( std::vector<LHCb::CondDBNameTagPair>& tags ) const {
  // first add the main db
  m_mainDB->defaultTags(tags);
  // loop over alternatives
  for (auto& alt : m_alternatives) alt.second->defaultTags(tags);
}

//=============================================================================
