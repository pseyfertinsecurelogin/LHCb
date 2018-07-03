#ifndef VELOPIXELINFO_H 
#define VELOPIXELINFO_H 1

// Include files

/** Constant information of the detector used to size the arrays
 *  
 *
 *  @author Renato Quagliani
 *  @date   2018-02-21
 *
 *  moved from REC/Pr/PrKernel/PrKernel to LHCB on 20180710
 */
namespace VeloInfo{
  enum Numbers{
    NModules = 52,
    NOffsets = 53, //NModules + 1
    NChipCorners = 6,
  };
}
#endif // VELOPIXELINFO_H
