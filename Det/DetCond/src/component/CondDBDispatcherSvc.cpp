// $Id: CondDBDispatcherSvc.cpp,v 1.7 2008-06-26 14:22:45 marcocle Exp $
// Include files

#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/Time.h"

// local
#include "CondDBDispatcherSvc.h"
#include "CondDBCommon.h"

// Factory implementation
DECLARE_SERVICE_FACTORY(CondDBDispatcherSvc)

//-----------------------------------------------------------------------------
// Implementation file for class : CondDBDispatcherSvc
//
// 2006-07-10 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CondDBDispatcherSvc::CondDBDispatcherSvc( const std::string& name, ISvcLocator* svcloc ):
  Service(name,svcloc),
  m_mainDB(0),
  m_alternatives()
{
  declareProperty("MainAccessSvc", m_mainAccessSvcName = "CondDBAccessSvc" );
  declareProperty("Alternatives",  m_alternativesDeclaration               );

  declareProperty("EnableXMLDirectMapping", m_xmlDirectMapping = true,
                  "Allow direct mapping from CondDB structure to"
                  " transient store.");
}

//=============================================================================
// Destructor
//=============================================================================
CondDBDispatcherSvc::~CondDBDispatcherSvc() {} 

//=============================================================================
// queryInterface
//=============================================================================
StatusCode CondDBDispatcherSvc::queryInterface(const InterfaceID& riid,
                                               void** ppvUnknown){
  if ( IID_ICondDBReader.versionMatch(riid) ) {
    *ppvUnknown = (ICondDBReader*)this;
    addRef();
    return SUCCESS;
  } else if ( IID_ICondDBInfo.versionMatch(riid) )   {
    *ppvUnknown = (ICondDBInfo*)this;
    addRef();
    return SUCCESS;
  }
  return Service::queryInterface(riid,ppvUnknown);
}

//=============================================================================
// initialize
//=============================================================================
StatusCode CondDBDispatcherSvc::initialize(){
  StatusCode sc = Service::initialize();
  if (sc.isFailure()) return sc;

  MsgStream log(msgSvc(), name() );

  log << MSG::DEBUG << "Initialize" << endmsg;
  
  // locate the main access service
  sc = service(m_mainAccessSvcName,m_mainDB,true);
  if (  !sc.isSuccess() ) {
    log << MSG::ERROR << "Could not locate " << m_mainAccessSvcName << endreq;
    return sc;
  }

  // locate all the alternative AccessSvcs
  std::vector<std::string>::iterator decl;
  for ( decl = m_alternativesDeclaration.begin(); decl != m_alternativesDeclaration.end(); ++decl ) {
    std::string::size_type pos = decl->find('=');
    if ( pos == std::string::npos ) {
      log << MSG::ERROR << "Malformed declaration of alternative: it sould be '/path/in/cool=ServiceType/ServiceName'" << endmsg;
      return StatusCode::FAILURE;
    }
    
    std::string svcName(*decl,pos+1);
    std::string altPath(*decl,0,pos);
    
    if ( m_alternatives.find(altPath) != m_alternatives.end() ) {
      log << MSG::ERROR << "More than one alternative for path " << altPath << endreq;
      return StatusCode::FAILURE;
    }

    ICondDBReader *svcPtr;
    sc = service(svcName,svcPtr,true);
    if (  !sc.isSuccess() ) {
      log << MSG::ERROR << "Could not locate " << svcName << endreq;
      return sc;
    }

    m_alternatives[altPath] = svcPtr;
    log << MSG::DEBUG << "Retrieved '" << svcName << "' (for path '" << altPath << "')" << endreq;

  }
  
  return sc;
}

//=============================================================================
// finalize
//=============================================================================
StatusCode CondDBDispatcherSvc::finalize(){
  MsgStream log(msgSvc(), name() );
  log << MSG::DEBUG << "Finalize" << endmsg;

  if (m_mainDB) {
    m_mainDB->release();
    m_mainDB = 0;
  }

  std::map<std::string,ICondDBReader*>::iterator alt;
  for ( alt = m_alternatives.begin(); alt != m_alternatives.end(); ++alt ) {
    if (alt->second) alt->second->release();
  }
  m_alternatives.clear();

  return Service::finalize();
}

//=========================================================================
//  find the appropriate alternative
//=========================================================================
ICondDBReader *CondDBDispatcherSvc::alternativeFor(const std::string &path) const {
  MsgStream log(msgSvc(), name() );

  log << MSG::VERBOSE << "Get alternative DB for '" << path << "'" << endmsg;
  if ( path.empty() || (path == "/") ) {
    log << MSG::VERBOSE << "Root node: using '" << m_mainAccessSvcName << "'" << endmsg;
    return m_mainDB;
  }
  
  // loop over alternatives
  std::map<std::string,ICondDBReader*>::const_reverse_iterator alt;
  for ( alt = m_alternatives.rbegin(); alt != m_alternatives.rend(); ++alt ) {
    if ( m_outputLevel <= MSG::VERBOSE ) {
      log << MSG::VERBOSE << "Comparing with " << alt->first << endmsg;
    }
    // FIXME: (MCl) wrong logic
    //     path=/Conditions/Velo/AlignmentCatalog.xml
    //     alt.=/Conditions/Velo/Alignment
    //     Should not match
    if ( ( path.size() >= alt->first.size() ) &&
         ( path.substr(0,alt->first.size()) == alt->first ) ){
      if ( m_outputLevel <= MSG::VERBOSE ) {
        IService *svc = dynamic_cast<IService*>(alt->second);
        log << MSG::VERBOSE << "Using '" ;
        if (svc) log << svc->name();
        else log << "unknown";
        log << "'" << endmsg;
      }
      
      return alt->second;
    }
  }

  log << MSG::VERBOSE << "Not found: using '" << m_mainAccessSvcName << "'" << endmsg;
  return m_mainDB;
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
  std::map<std::string,ICondDBReader*>::reverse_iterator alt;
  std::string::size_type path_size = path.size();
  for ( alt = m_alternatives.rbegin(); alt != m_alternatives.rend(); ++alt ) {
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
// Collect the list of used tags and databases
//=========================================================================
void CondDBDispatcherSvc::defaultTags ( std::vector<LHCb::CondDBNameTagPair>& tags ) const {
  // first add the main db
  m_mainDB->defaultTags(tags);

  // loop over alternatives
  std::map<std::string,ICondDBReader*>::const_iterator alt;
  for ( alt = m_alternatives.begin(); alt != m_alternatives.end(); ++alt ) {
    alt->second->defaultTags(tags);
  }
}


//=============================================================================
