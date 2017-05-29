#ifndef DETDESC_CONDITIONINFO_H
#define DETDESC_CONDITIONINFO_H 1

#include <string>

#include "DetDesc/IConditionInfo.h"
// Base class
#include "DetDesc/Services.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/StatusCode.h"

class Condition;
// Forward declarations
class IDataProviderSvc;
struct IDetectorElement;

/**
 *  @class ConditionInfo ConditionInfo.h Det/DetDesc/ConditionInfo.h
 *
 *  The simplest implementation of the IConditionInfo abstract interface.
 *
 *  @author Andrea Valassi
 *  @author Sebastien Ponce
 *
 */

class ConditionInfo : virtual public IConditionInfo
{

 public:

  /// Constructor
  ConditionInfo( IDetectorElement* de, const std::string& condition );

 private:

  /// Get a pointer to the data service responsible for condition data
  IDataProviderSvc* dataSvc() const;

 public:

  // Implementation of the IConditionInfo interface

  /// Get a pointer to the detector element to which the ConditionInfo belongs
  IDetectorElement* detElem() const override;

  /// Get the name of the associated condition
  const std::string& conditionName() const override;

  /// Get a pointer to the associated condition
  Condition* condition() override;

  // Implementation of the IInterface interface

  /// query interface
  StatusCode queryInterface( const InterfaceID& ID , void** ppI ) override;
  /// add reference
  unsigned long addRef() override;
  /// release
  unsigned long release() override;


 private:

  /// Pointer to the detector element to which the ConditionInfo belongs
  IDetectorElement* m_detElem;

  /// Name of the associated condition
  std::string m_conditionName;

  /// Pointer to the associated condition (loaded on demand only)
  Condition* m_condition;

  /// reference count
  unsigned long m_count;

  /// reference to dataSvc
  DetDesc::ServicesPtr m_services;

};

#endif // DETDESC_CONDITIONINFO_H
