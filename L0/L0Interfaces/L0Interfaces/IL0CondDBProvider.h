#ifndef IL0CONDDBPROVIDER_H 
#define IL0CONDDBPROVIDER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class IL0CondDBProvider IL0CondDBProvider.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-10-13
 */
struct IL0CondDBProvider : extend_interfaces<IAlgTool> {

  // Declare the interface ID
  DeclareInterfaceID( IL0CondDBProvider, 2, 0 );

  virtual double scale(unsigned int base)=0;
  virtual double caloEtScale()=0;
  virtual double muonPtScale()=0;
  virtual const std::vector<int> RAMBCID(std::string vsn)=0;
  virtual int RAMBCID(std::string vsn,int bcid)=0;
  
};
#endif // IL0CONDDBPROVIDER_H
