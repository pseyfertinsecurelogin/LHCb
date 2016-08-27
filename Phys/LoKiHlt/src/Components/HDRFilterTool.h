#ifndef HDRFILTERTOOL_H
#define HDRFILTERTOOL_H 1

// STL
#include <string>

// HltEvent
#include "Event/HltDecReports.h"
// LoKi
#include "LoKi/FilterAlg.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/IHltFactory.h"

namespace LoKi {
/** @class HDRFilterTool HDRFilterTool.h
 *
 *   HDR Filter Tool class to filter efficiently execute filters on lines
 *
 *   Input:
 *    - InputHltDecReportsLocation : Decision Reports location
 *    - KillFromAll : Main behaviour switch. If false (default), kill only banks in the first location found in the search string. If true, kill *all* banks found in the search string.
 *    - DecisionFilter : Regular expression to filter input. Lines that do not match this filter are ignored. By default it accepts all lines.
 *    - AlwaysKeepBanks: Set of Banks to always keep regardless of line conditions.
 *
 *    @author: Konstantin Gizdov
 *    @date:   26 August 2016
 */
class HDRFilterTool : public LoKi::FilterAlg {
  /// friend factory for instantiation
  friend class AlgFactory<LoKi::HDRFilterTool>;

 public:
  /// initialization
  virtual StatusCode initialize();
  /// the main method: pass
  bool pass(const LHCb::HltDecReports * hdr);

  /** Decode the functor (use the factory)
   *  @see LoKi::FilterAlg
   *  @see LoKi::FilterAlg::decode
   *  @see LoKi::FilterAlg::i_decode
   */
  StatusCode decode();

 protected:
  /** standard constructor
   *  @see LoKi::FilterAlg
   *  @see GaudiAlgorithm
   *  @see      Algorithm
   *  @see      AlgFactory
   *  @see     IAlgFactory
   *  @param name the algorithm instance name
   *  @param pSvc pointer to Service Locator
   */
  HDRFilterTool(const std::string &name,   // the algorithm instance name
                ISvcLocator       *pSvc);  // pointer to the service locator
  /// virtual and protected destructor
  virtual ~HDRFilterTool() {}

 private:
  /// the default constructor is disabled
  HDRFilterTool();
  /// the copy constructor is disabled
  HDRFilterTool(const HDRFilterTool &);
  /// the assignement operator is disabled
  HDRFilterTool& operator=(const HDRFilterTool &);

  /// the functor itself
  LoKi::Types::HLT_Cut  m_cut;
};
}  // namespace LoKi

#endif  // HDRFILTERTOOL_H
