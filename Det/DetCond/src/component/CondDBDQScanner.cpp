// Include files

#include <CoolKernel/IField.h>
#include <functional>
#include <map>
#include <memory>
#include <typeinfo>

// local
#include "CondDBDQScanner.h"
#include "CoolKernel/RecordException.h"
// From LHCb
#include "DetCond/ICondDBReader.h"
#include "DetDesc/Condition.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/DataObject.h"
// From Gaudi
#include "GaudiKernel/IConverter.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IService.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/Time.icpp"
#include "RelyConverter.h"

class IInterface;

// ----------------------------------------------------------------------------
// Implementation file for class: CondDBDQScanner
//
// 04/11/2011: Marco Clemencic
// ----------------------------------------------------------------------------
DECLARE_TOOL_FACTORY(CondDBDQScanner)

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
CondDBDQScanner::CondDBDQScanner(const std::string& type, const std::string& name, const IInterface* parent)
  : base_class(type, name, parent)
{

  declareProperty("ConditionPath",
                  m_condPath = "/Conditions/DQ/Flags",
                  "Path in the Conditions Database where to find the Data "
                  "Quality condition.");

  declareProperty("CondDBReader",
                  m_condDBReaderName = "CondDBCnvSvc",
                  "Service implementing the ICondDBReader interface to be used "
                  "to access the CondDB.");

  declareProperty("Converter",
                  m_converterName = "DetectorPersistencySvc",
                  "Service implementing the IConverter interface.");
}

CondDBDQScanner::~CondDBDQScanner() {}

IDQFilter::FlagsType CondDBDQScanner::scan(const Gaudi::Time & since, const Gaudi::Time & until) const
{
  typedef ICondDBReader::IOVList IOVList;
  typedef ICondDBReader::IOV IOV;
  IDQFilter::FlagsType flags;

  ICondDBReader::DataPtr data;
  Gaudi::Time dataSince, dataUntil;
  std::string desc;

  // Loop over the list of conditions in the folder
  IOVList iovs = m_condDB->getIOVs(m_condPath, IOV(since, until));
  for(IOVList::iterator iov = iovs.begin(); iov != iovs.end(); ++iov) {
    // get the condition data (XML)
    StatusCode sc = m_condDB->getObject(m_condPath, iov->since, data, desc, dataSince, dataUntil);
    if (sc.isFailure()){
      Exception("Problems retrieving data from the database");
      return flags; // never reached, but helps Coverity
    }

    try {
      // prepare the IOpaqueAddress to be given to the PersistencySvc
      const long storageType = RelyConverter::getStorageType(m_condPath, desc);
      const std::string xml_data = (*data.get())["data"].data<std::string>();
      std::unique_ptr<IOpaqueAddress> addr{  RelyConverter::createTmpAddress("conddb:" + m_condPath,
                                                             storageType,
                                                             "Flags",
                                                             Condition::classID(),
                                                             xml_data,
                                                             info(),
                                                             m_converter->addressCreator()) };
      if (!addr){
        Exception("Failed to create temporary IOpaqueAddress");
        return flags; // never reached, but helps Coverity
      }

      // Retrieve the condition data
      DataObject *obj = nullptr;
      Condition *cond = nullptr;
      if (m_converter->createObj(addr.get(), obj).isFailure()
          || m_converter->fillObjRefs(addr.get(), obj).isFailure()
          || (cond = dynamic_cast<Condition*>(obj)) == nullptr) { //assignment intended
        addr.reset();
        delete obj;
        Exception("Conversion of Condition failed");
        return flags; // never reached, but helps Coverity
      }

      // Merge the condition map with the collected one.
      const IDQFilter::FlagsType &condFlags = cond->param<IDQFilter::FlagsType>("map");
      flags.insert(condFlags.begin(), condFlags.end());

      delete obj;

    } catch (cool::RecordSpecificationUnknownField &e) {
      Exception(std::string("I cannot find the data inside COOL object: ") + e.what());
    }
  }

  return flags;
}




StatusCode CondDBDQScanner::initialize()
{
  StatusCode sc = base_class::initialize();
  if (sc.isFailure()) return sc;

  m_condDB = service(m_condDBReaderName);
  if (UNLIKELY(!m_condDB.isValid())) {
    error() << "Cannot get the ICondDBReader implementation " << m_condDBReaderName << endmsg;
    return StatusCode::FAILURE;
  }

  m_converter = service(m_converterName);
  if (UNLIKELY(!m_converter.isValid())) {
    error() << "Cannot get the IConverter implementation " << m_converterName << endmsg;
    return StatusCode::FAILURE;
  }

  return sc;
}



StatusCode CondDBDQScanner::finalize()
{
  m_condDB.reset(); // release the ICondDBReader service

  return base_class::finalize();
}



// ============================================================================
