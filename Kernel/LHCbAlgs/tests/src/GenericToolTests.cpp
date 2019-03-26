/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
/** Examples and tests of use of IGenericThreadSafeTool interface.
 */
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/Algorithm.h"
#include "Kernel/IGenericThreadSafeTool.h"

namespace LHCbTests {
  namespace GenericTool {
    struct MyGenericTool : public extends<AlgTool, IGenericTool> {
      using extends::extends;

      void execute() override { info() << "MyGenericTool::execute()" << endmsg; }
    };
    DECLARE_COMPONENT( MyGenericTool )

    struct MyGenericTSTool : public extends<AlgTool, IGenericThreadSafeTool> {
      using extends::extends;
      using IGenericThreadSafeTool::execute;

      void execute() const override { info() << "MyGenericTSTool::execute() const" << endmsg; }
    };
    DECLARE_COMPONENT( MyGenericTSTool )

    struct Algorithm : public ::Algorithm {
      using ::Algorithm::Algorithm;

      StatusCode initialize() override {
        auto sc = ::Algorithm::initialize();
        if ( !sc ) return sc;
        info() << "getting the tools..." << endmsg;
        sc = StatusCode{m_gt1.retrieve() && m_gt2a.retrieve() && m_gt2b.retrieve()};
        return sc;
      }
      StatusCode execute() override {
        info() << "executing..." << endmsg;
        m_gt1->execute();
        m_gt2a->execute();
        m_gt2b->execute();
        i_constMethod();
        info() << "...done" << endmsg;
        return StatusCode::SUCCESS;
      }
      StatusCode finalize() override {
        m_gt1.release().ignore();
        m_gt2a.release().ignore();
        m_gt2b.release().ignore();
        return ::Algorithm::finalize();
      }

    private:
      ToolHandle<IGenericTool>           m_gt1{"LHCbTests::GenericTool::MyGenericTool/Generic", this};
      ToolHandle<IGenericTool>           m_gt2a{"LHCbTests::GenericTool::MyGenericTSTool/GenericTS", this};
      ToolHandle<IGenericThreadSafeTool> m_gt2b{"LHCbTests::GenericTool::MyGenericTSTool/GenericTS", this};

      void i_constMethod() const {
        info() << "from const method..." << endmsg;
        // m_gt1->execute(); // not allowed
        // m_gt2a->execute(); // not allowed
        m_gt2b->execute(); // not allowed
      }
    };
    DECLARE_COMPONENT( Algorithm )
  } // namespace GenericTool
} // namespace LHCbTests
