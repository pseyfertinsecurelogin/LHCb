// Class Header
#include "HDRFilterTool.h"

/** @file HDRFilterTool.cpp
 *
 *    @author: Konstantin Gizdov
 *    @date:   26 August 2016
 */

/*  standard constructor
 *  @see LoKi::FilterAlg
 *  @see GaudiAlgorithm
 *  @see      Algorithm
 *  @see      AlgFactory
 *  @see     IAlgFactory
 *  @param name the algorithm instance name
 *  @param pSvc pointer to Service Locator
 */
LoKi::HDRFilterTool::HDRFilterTool(const std::string &name,  // the algorithm instance name
                                   ISvcLocator       *pSvc)  // pointer to the service locator
                          : LoKi::FilterAlg(name, pSvc),
                          // the functor itself
                          m_cut(LoKi::BasicFunctors<const LHCb::HltDecReports*>::BooleanConstant(false)) {
  StatusCode sc = setProperty("Code", "HLT_NONE");
  Assert(sc.isSuccess(), "Unable (re)set property 'Code'", sc);
  sc = setProperty("Factory",
                   0 == name.find("Hlt1") ?
                   "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC" :
                    (0 == name.find("Hlt2") ?
                     "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC" :
                     "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC"));
  Assert(sc.isSuccess(), "Unable (re)set property 'Factory'", sc);
}

StatusCode LoKi::HDRFilterTool::decode() {
      StatusCode sc = i_decode<LoKi::Hybrid::IHltFactory>(m_cut);
      Assert(sc.isSuccess(), "Unable to decode the functor!");
      return StatusCode::SUCCESS;
}

// initialization
StatusCode LoKi::HDRFilterTool::initialize() {
  StatusCode sc = LoKi::FilterAlg::initialize();
  if (sc.isFailure()) return sc;
  return sc;
}

// the main method: pass
bool LoKi::HDRFilterTool::pass(const LHCb::HltDecReports * hdr) {
  StatusCode sc = decode();
  Assert(sc.isSuccess(), "Unable to decode the functor!");

  // use the functor
  return m_cut(hdr);
}

/// the factory (needed for instantiation)
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LoKi, HDRFilterTool)
