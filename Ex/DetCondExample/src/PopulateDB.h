#ifndef POPULATEDB_H 
#define POPULATEDB_H 1

#include <string>

#include "DetCond/ICondDBAccessSvc.h"
#include "DetCond/ICondDBEditor.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"

class Condition;
class ICondDBAccessSvc;
// Forward declarations
class ICondDBEditor;
class ICondDBObject;
class ISvcLocator;

/** @class PopulateDB PopulateDB.h
 *  
 *  Simple algorithm to populate the ConditionsDB.
 *
 *  @author Marco Clemencic
 *  @date   2005-06-22
 */
class PopulateDB : public GaudiAlgorithm {
public: 
  /// Standard constructor
  PopulateDB( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  /// Store sample data
  StatusCode i_condDBStoreSampleData();
  
  StatusCode i_createHierarchy();
  StatusCode i_createCOLDVersion();
  StatusCode i_createHOTVersion();
  StatusCode i_createHEADVersion();

  /// Dump sample data
  StatusCode i_condDBDumpSampleData() const;

  /// Encode XML temperature data
  std::string i_encodeXmlTemperature( const double temperature,
                                      const std::string& objName ) const;
  
  /// Encode XML paramVector
  std::string i_encodeXmlParamVector( const double par[3],
                               const std::string& objName,
                               const std::string& parName ) const;
  
  /// Dump the contents of a CondDBFolder
  StatusCode i_dumpFolder( const std::string& folderName,
                           const std::string& tagName ) const;
  
  /// Handle to the CondDBAccessSvc (to write the data in the CondDB)
  SmartIF<ICondDBEditor> m_dbEditor;

  /// Handle to the CondDBAccessSvc (to access cool database interface)
  SmartIF<ICondDBAccessSvc> m_dbAccSvc;

};
#endif // POPULATEDB_H
