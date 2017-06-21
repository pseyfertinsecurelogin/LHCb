// ===========================================================================
#ifndef     CALOUTILS_CALODATA_H
#define     CALOUTILS_CALODATA_H 1 
// ===========================================================================
// from STL 
#include <string>
// from Gaudi 
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatusCode.h" 
#include "GaudiKernel/SmartDataPtr.h"
// from CaloKernel
#include "CaloKernel/CaloException.h"
// From CaloEvent 
#include "Event/CaloDigit.h"

class  IMessageSvc; 
class  IDataProviderSvc; 

/** @namespace CaloData   CaloData.h CaloAlgs/CaloData.h
 * 
 *  simple utility functions to get the calorimeter data in an easy way 
 *
 *  @author: Vanya Belyaev Ivan.Belyaev@itep.ru 
 *  @date:   Dec 1,   1999
 */

namespace CaloData
{
  /** a simple utility to get digits
   *  @param  dataService    data provider        
   *  @param  address        address(path) in transient store 
   *  @param  output         output iterator    
   *  @param  messageService (obsolete) message service pointer 
   */
  template< class OUTPUTTYPE>
  StatusCode Data( IDataProviderSvc*  dataService         ,  
                   const std::string& address             , 
                   OUTPUTTYPE::CONTAINER   output              , 
                   IMessageSvc*       messageService  = 0 ) 
  {
    ///
    IMessageSvc* aux = nullptr  ; 
    aux  = messageService ; // trick to avoid comopilation warnings 
    /// check for data service 
    if( !dataService ) 
      { 
        throw CaloException ("CaloData:: IDataProviderSvc* poins to NULL!"); 
        return StatusCode::FAILURE ; 
      }
    /// retrive data from the store 
    SmartDataPtr< ObjectVector<OUTPUTTYPE> > smart( dataService , address ); 
    /// success??
    if( !smart )           
      { throw CaloException("CaloData::  unable to retrieve "+address ); 
      return StatusCode::FAILURE;  }
    /// copy data using output iterator, remove nulls   
    std::remove_copy( smart->begin() , smart->end() , 
                      OUTPUTTYPE::CONTAINER , (OUTPUTTYPE *) 0 ); 
    return StatusCode::SUCCESS;                           /* RETURN! */  
    }
  ///
} // end of namespace CaloData

#endif  //  CALOUTILS_CALODATA_H
