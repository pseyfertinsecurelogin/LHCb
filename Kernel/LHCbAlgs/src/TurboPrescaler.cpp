// Include files 

// from Gaudi
#include "GaudiKernel/IAlgManager.h"
#include "GaudiAlg/GaudiSequencer.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "Kernel/IANNSvc.h"
#include "DeterministicPrescaler.h"
// local
#include "TurboPrescaler.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : TurboPrescaler
//
// Description: The class takes an output TCK and applies
// an additional prescale
//
// 2016-06-13 : Sean Benson
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( TurboPrescaler )


TurboPrescaler::TurboPrescaler( const std::string& name,
    ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("ChosenOutputPrescales",m_outputPS=std::map<std::string,double>());
  declareProperty("PrescaleVersion",m_outputTCK);
  declareProperty("FilterOutput",m_filter=true);
  declareProperty("ReportsOutputLoc",m_outRepLoc="Turbo/DecReports");
  declareProperty("ConfigQualifier",m_scalerName="DeterministicPrescaler");
  declareProperty("PreScalerQualifier",m_preScalerName="PreScaler");
  declareProperty("PostScalerQualifier",m_postScalerName="PostScaler");
  declareProperty("HltDecReportsLocation",m_hltDecReportsLocation=HltDecReportsLocation::Default);
  declareProperty("IPropertyConfigSvcInstance", m_propertyConfigSvcName = "PropertyConfigSvc");

}

StatusCode TurboPrescaler::initialize() {
  StatusCode sc = StatusCode::SUCCESS;

  // Get property config service
  if (!service( m_propertyConfigSvcName, m_propertyConfigSvc).isSuccess()) {
    fatal() << "Failed to get the IConfigAccessSvc." << endmsg;
    return StatusCode::FAILURE;
  }
  setupPrescalers();    

  return sc;
}

StatusCode TurboPrescaler::execute() {
  StatusCode sc = StatusCode::SUCCESS;

  // Get the input reports and if the TCK changes, refresh list of prescales
  auto decreports = getIfExists<HltDecReports>(m_hltDecReportsLocation);
  if(decreports){

    auto tck = decreports->configuredTCK();
    if(m_lastTCK==0) {
      getPrescalesFromTCK(tck,m_prescalesInput, m_postscalesInput, m_scaleProductsInput);
    }

    if(tck!=m_lastTCK||m_lastTCK==0){
      m_lastTCK=tck;
      updatePrescalers();
    }

    bool globalPass=false;

    HltDecReports* reports = new HltDecReports();
    put(reports,m_outRepLoc);


    reports->setConfiguredTCK(m_outputTCK);
    reports->setTaskID( decreports->taskID() );
    HltDecReport report;

    std::string lineName;
    std::string totName;
    std::string strip= "Decision";

    //loop over the Decreports
    for(const auto& dec : *decreports) {

      //Get the name of line, if it ends with "Decision" remove it: 
      lineName =  dec.first;
      totName=lineName;
      if(endedWith(lineName,strip)){lineName.erase(lineName.end()-strip.length(),lineName.end());}

      //Get the report
      report = dec.second;

      //We only care about lines that fired: 
      if(report.decision()){

        //Pull the DeterministicPrescaler for this line from out map of DPs: 
        Algorithm* prescaler= prescalers[lineName];

        // Execute prescaler
        if(prescaler && prescaler->isEnabled()){
          StatusCode result = prescaler->sysExecute();
          if ( ! result.isSuccess() ) return StatusCode::FAILURE;
          //If the DP wants the line killed: 
          if(!prescaler->filterPassed()){
            report.setDecision(false);
          }
          else {
            globalPass=true;
          }
        }
      }
      reports->insert(totName,report);
    }

    if(m_filter){
      setFilterPassed(globalPass);
    }
    else{
      setFilterPassed(true);
    }
  }
  return sc;
}

void TurboPrescaler::getPrescalesFromTCK(unsigned int tck, std::map<std::string, double> &prescales, std::map<std::string, double> &postscales, std::map<std::string, double> &scaleProducts){
  prescales.clear();
  postscales.clear();
  scaleProducts.clear();
  TCK _tck(tck);
  _tck.normalize();

  ConfigTreeNodeAlias::alias_type alias( std::string("TCK/") +  _tck.str()  );
  const ConfigTreeNode* tree = m_propertyConfigSvc->resolveConfigTreeNode(alias); 
  if (!tree) {
    fatal() << "TCK " << std::hex << _tck << " could not be resolved! Cannot continue" << endmsg;
    return ;
  }

  const std::vector<PropertyConfig::digest_type>& digests =  m_propertyConfigSvc->collectLeafRefs( tree->digest() );
  std::string lineName;
  for(std::vector<PropertyConfig::digest_type>::const_iterator j = digests.begin(); j!= digests.end(); ++j){
    const PropertyConfig* config = m_propertyConfigSvc->resolvePropertyConfig( *j );
    if(config){

      //Check that fullyQualifiedName contains "DeterministicPrescaler"
      if(std::string::npos != config->fullyQualifiedName().find(m_scalerName)){

        //If it does, get the name
        lineName = config->name();
        if(lineName.find("Turbo")==std::string::npos) continue;

        //Loop to find the property "AcceptFraction" 
        for(PropertyConfig::Properties::const_iterator i =  config->properties().begin(); i!= config->properties().end(); ++i ) {
          if(i->first == "AcceptFraction"){
            double scale = -9999.;
            try {
              std::string str2 = (i->second);
              scale = std::strtod(str2.c_str(), 0);
            }catch (const std::exception&){
              Warning("could not find pre/post scale in " + i->first).ignore();
              scale=-9999.;
            }
            if(scale>-9999.){
              // Is it a Prescale, Postscale, or something else? 
              //prescale:
              if(endedWith(lineName,m_preScalerName)){
                lineName.erase(lineName.end()-m_preScalerName.length(),lineName.end());
                prescales[lineName]=scale; //Write the prescale to a map
                postscales.insert(std::pair<std::string,double>(lineName,-1.0)); //Initialise a postscale if it isn't already there with a nonsense value
                break;
              }else{
                //postscale:
                if(endedWith(lineName,m_postScalerName)){
                  lineName.erase(lineName.end()-m_postScalerName.length(),lineName.end());
                  postscales[lineName]=scale; //Write the postscale to a map
                  prescales.insert(std::pair<std::string,double>(lineName,-1.0)); //Initialise a prescale if it isn't already there with a nonsense value
                  break;
                }else{
                  Warning("Found property AcceptFraction not associated to a pre/post scale! " + lineName).ignore();
                }
              }

            }
          }

        }

      }
    }
  }

  //Make products of pre, post scales, print (if verbose)
  for( std::map<std::string,double>::iterator i=prescales.begin(); i!=prescales.end(); ++i){
    double pre = (*i).second;
    if(pre<0.0){pre=1.0;} //If prescale was never found it'll be -9999., and we assume it will always pass
    double post = postscales[(*i).first];
    if(post<0.0){post=1.0;} //If postscale was never found it'll be -9999., and we assume it will always pass
    scaleProducts.insert(std::pair<std::string,double>((*i).first,pre*post));
  }
}

void TurboPrescaler::setupPrescalers(){
  prescalers.clear();

  //== Get the "Context" option if in the file...
  auto jos = service<IJobOptionsSvc>("JobOptionsSvc");

  //= Get the Application manager, to see if algorithm exist
  auto appMgr = service<IAlgManager>("ApplicationMgr");
  
  for( std::map<std::string,double>::iterator it=m_outputPS.begin(); it!=m_outputPS.end(); ++it){
    std::string tn = "DeterministicPrescaler/";
    tn.append(it->first);
    tn.append("DPForPrescaleOffline");
    const Gaudi::Utils::TypeNameString typeName(tn);
    const std::string &theName = typeName.name();
    const std::string &theType = typeName.type();

    //== Check wether the specified algorithm already exists. If not, create it
    StatusCode result = StatusCode::SUCCESS;
    SmartIF<IAlgorithm> myIAlg = appMgr->algorithm(typeName, false); // do not create it now

    if ( !myIAlg.isValid() ) {

      //Set the acceptfraction and verbosity for the DP: 
      if(UNLIKELY( msgLevel(MSG::VERBOSE)))verbose() << "Setting an acceptfraction for " << theName << endmsg;
      IntegerProperty outputLevelProperty( "OutputLevel", MSG::WARNING);
      jos->addPropertyToCatalogue( theName, outputLevelProperty ).ignore();

      DoubleProperty acceptFractionProperty( "AcceptFraction", it->second);
      jos->addPropertyToCatalogue( theName, acceptFractionProperty ).ignore();

      Algorithm *myAlg = 0;
      result = createSubAlgorithm( theType, theName, myAlg );
      if (result.isSuccess()) myIAlg = myAlg;
    }

    // propagate the sub-algorithm into own state.
    if ( result.isSuccess() )
    {
      StatusCode sc = myIAlg->sysInitialize() ;
      if  ( sc.isFailure() ) { result = sc ; }
    }

    // propagate the sub-algorithm into own state.
    if ( result.isSuccess () )
    {
      StatusCode sc = myIAlg->sysStart () ;
      if  ( sc.isFailure() ) { result = sc ; }
    }
    if ( result.isSuccess() ) {
      Algorithm*  myAlg = dynamic_cast<Algorithm*>(myIAlg.get());
      if (myAlg!=0) {
        prescalers.insert(std::pair<std::string,Algorithm*>(it->first,myAlg));
      } else {
        warning() << theName << " is not an Algorithm - failed dynamic_cast"
          << endmsg;
      }
    } else {
      warning() << "Unable to find or create " << theName << endmsg;
    }
  }

}

void TurboPrescaler::updatePrescalers(){
  for( auto i=m_prescalesInput.begin(); i!=m_prescalesInput.end(); ++i){
    DeterministicPrescaler* pre = dynamic_cast<DeterministicPrescaler*>(prescalers[(*i).first]);
    
    if(!pre) continue;
    
    auto j = m_outputPS.find((*i).first);
    if(j!=m_outputPS.end()){
      if((*j).second > 0.0){
        double ratio = (*j).second/(*i).second;
        if(ratio<=1.0){
          DoubleProperty acceptFractionProperty( "AcceptFraction", ratio);
          pre->setProperty(acceptFractionProperty);
        }else{
          warning() << "*********************************************************************************" << endmsg;
          warning() << "OUTPUT HAS LARGER PRESCALE THAN INPUT, CANNOT CREATE EVENTS. LEAVING AS UNITY" << endmsg;
          warning() << "*********************************************************************************" << endmsg;
          DoubleProperty acceptFractionProperty( "AcceptFraction", 1.0);
          pre->setProperty(acceptFractionProperty);
        }
      }
    }
  }
}

bool TurboPrescaler::endedWith(const std::string &lineName, const std::string &ending){
  return lineName.length()>ending.length() && 
    (0 == lineName.compare (lineName.length() - ending.length(), ending.length(), ending));
}

