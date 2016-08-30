#ifndef HDRFILTERTOOL_H
#define HDRFILTERTOOL_H 1

// STL
#include <string>

// HltEvent
#include "Event/HltDecReports.h"
// LoKi
#include "LoKi/FilterTool.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/IHltFactory.h"

namespace LoKi {
/** @class HDRFilterTool HDRFilterTool.h
 *
 *  Simple filtering tool based on LoKi/Bender "hybrid" approach for filtering according to LHCb::HltDecReports.
 *
 *  @author: Konstantin Gizdov
 *  @date:   26 August 2016
 */
class HDRFilterTool : public LoKi::FilterTool {
public:
  /// InterfaceID
  DeclareInterfaceID(HDRFilterTool, 1, 0);

  /// the main method: pass
  bool pass(const LHCb::HltDecReports * hdr) const;

  /** Decode the functor (use the factory)
   *  @see LoKi::FilterTool
   *  @see LoKi::FilterTool::decode
   *  @see LoKi::FilterTool::i_decode
   */
  StatusCode decode();

protected:
  /// standard constructor
  HDRFilterTool(const std::string& type,   // tool type (?)
                const std::string& name,   // toolinstance name
                const IInterface* parent); // tool's parent
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

  /// friend factory for instantiation
  friend class ToolFactory<LoKi::HDRFilterTool>;

};
}  // namespace LoKi

#endif  // HDRFILTERTOOL_H
