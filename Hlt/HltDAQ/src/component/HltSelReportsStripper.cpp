#include <algorithm>

// Include files
#include "Event/HltSelReports.h"
#include "Event/HltObjectSummary.h"

// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : HltSelReportsStripper
//
// 2017-05-20 : 
//-----------------------------------------------------------------------------

class HltSelReportsStripper
  : public Gaudi::Functional::Transformer< LHCb::HltSelReports(const LHCb::HltSelReports&),
					   Gaudi::Functional::Traits::BaseClass_t<GaudiHistoAlg> >
{
  
public:
  //=============================================================================
  // Standard constructor, initializes variables
  //=============================================================================
  HltSelReportsStripper(const std::string& name, ISvcLocator* pSvcLocator)
    : Transformer( name, pSvcLocator,
		   KeyValue{ "Input", LHCb::HltSelReportsLocation::Default },
		   KeyValue{ "Output", LHCb::HltSelReportsLocation::Default })
  {
    
  }
  
  //=============================================================================
  // Standard constructor, initializes variables
  //=============================================================================
  LHCb::HltSelReports operator()(const LHCb::HltSelReports& inputSelReports) const override
  {
    // check input
    // if (!inputSelReports) {
    //   return Warning( " No input HltSelReports ", StatusCode::SUCCESS, 20 );
    // }


    // --------------------------------------------------------
    std::cout << " Input HltSelReports : { \n" ;
    for(const auto& selReport : inputSelReports) {
      std::cout << " Name : " << selReport.first
		<< " HltObjectSummary : " << selReport.second
		<< "\n"; 
    }
    std::cout << " }" << std::endl;
    // --------------------------------------------------------

    // --------------------------------------------------------
    std::cout << " Selecting selReports from : { \n" << std::endl;
    for (auto line : m_lines){
      std::cout << " selectionName : " << line << "\n";
    }
    std::cout << " } " << std::endl;
    std::cout << " [...] " << std::endl;

    LHCb::HltSelReports outputSelReports;
    for(const auto& selReport : inputSelReports) {
      if (std::find(std::begin(m_lines), std::end(m_lines), selReport.first) !=
          std::end(m_lines))
        outputSelReports.insert(selReport.first, selReport.second);
    }
    // --------------------------------------------------------
    
    // --------------------------------------------------------
    std::cout << " Output HltSelReports : { \n" ;
    for(const auto& selReport : outputSelReports) {
      std::cout << " Name : " << selReport.first
		<< " HltObjectSummary : " << selReport.second
		<< "\n"; 
    }
    std::cout << " }" << std::endl;
    // --------------------------------------------------------

    
    return outputSelReports;
  }
  
private:
  Gaudi::Property< std::vector<std::string> > m_lines{this, "Lines"};  

}; 

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltSelReportsStripper )

//=============================================================================
