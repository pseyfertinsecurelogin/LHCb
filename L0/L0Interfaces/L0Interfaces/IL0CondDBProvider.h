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

  virtual double scale(unsigned int base) const =0;
  virtual double caloEtScale() const =0;
  virtual double muonPtScale() const = 0;
  virtual const std::vector<int> RAMBCID(std::string vsn) const =0;
  virtual int RAMBCID(std::string vsn,int bcid) const =0;

};
#endif // IL0CONDDBPROVIDER_H
