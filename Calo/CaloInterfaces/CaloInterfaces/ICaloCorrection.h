// ============================================================================
#ifndef CALOINTERFACES_ICALOCORRECTION_H 
#define CALOINTERFACES_ICALOCORRECTION_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"
#include "Event/CaloPosition.h"

/** @class ICaloCorrection ICaloCorrection.h CaloInterfaces/ICaloCorrection.h
 *  
 *  A minimal abstract interface for S-,L-shape correction functions 
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   22/03/2002
 */



struct ICaloCorrection : extend_interfaces<IAlgTool>
{
  
  DeclareInterfaceID( ICaloCorrection , 2 , 0 );
  
  /** The main processing method 
   *  @param  position  uncorrected position
   *  @param  seed      position of the 'seed' 
   *  @param  corrected new created corrected CaloPosition object 
   *  @return status code 
   */  
  virtual StatusCode     process    
  ( const LHCb::CaloPosition    & position  , 
    const LHCb::CaloPosition::Center & seed      , 
    LHCb::CaloPosition*&        corrected ) const = 0 ;
  
  /** The main processing method 
   *  @param  position  uncorrected position
   *  @param  seed      position of the 'seed' 
   *  @return new created corrected CaloPosition object 
   */  
  virtual LHCb::CaloPosition* process    
  ( const LHCb::CaloPosition & position , 
    const LHCb::CaloPosition::Center & seed     ) const = 0 ;
  
  /** The main processing method (functor interface)
   *  @param  position  uncorrected position
   *  @param  seed      position of the 'seed' 
   *  @return new created corrected CaloPosition object 
   */  
  virtual LHCb::CaloPosition* operator() 
    ( const LHCb::CaloPosition & position , 
      const LHCb::CaloPosition::Center & seed     ) const = 0 ;

};

// ============================================================================
#endif // CALOINTERFACES_ICALOCORRECTION_H
// ============================================================================
