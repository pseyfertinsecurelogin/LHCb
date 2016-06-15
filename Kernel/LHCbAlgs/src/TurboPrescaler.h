#ifndef TURBOPRESCALER_H 
#define TURBOPRESCALER_H 1

// Include files
// from DaVinci, this is a specialized GaudiAlgorithm
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/HltDecReports.h"
using namespace LHCb;

/** @class TurboPrescaler TurboPrescaler.h
 *  
 *
 *  @author Sean Benson
 *  @date   2016-06-13
 */
class TurboPrescaler : public GaudiAlgorithm {
    public: 
	/// Standard constructor
	TurboPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

	virtual ~TurboPrescaler( ); ///< Destructor

	virtual StatusCode initialize();    ///< Algorithm initialization
	virtual StatusCode execute   ();    ///< Algorithm execution

    protected:

    private:
    	std::string m_scalerName; //< Name prepended to PropertyConfig 
	std::string m_postScalerName; //< Name postpended to PropertyConfig if postscaled
	std::string m_preScalerName; //< Name postpended to PropertyConfig if prescaled
	
	StatusCode getPrescalesFromTCK(unsigned int, std::map<std::string, double> &, std::map<std::string, double> &, std::map<std::string, double> &); ///< Fill maps with prescale, postscale and post*prescale based on specificed TCK
	bool endedWith(const std::string &lineName, const std::string &ending);

	StatusCode setupPrescalers(); ///< Initialise Prescalers
	StatusCode updatePrescalers(); ///< Set accept rates of Prescalers based on ratio of prescales from the user-specced TCK and the TCK in (the first, or changed) MC event TCK


	std::map<std::string, double> m_prescalesInput; //name of prescalers
	std::map<std::string, double> m_postscalesInput; //name of postscalers
	std::map<std::string, double> m_scaleProductsInput; //pre*post-scales 
	std::map<std::string, double> m_prescalesOutput; //name of prescalers
	std::map<std::string, double> m_postscalesOutput; //name of postscalers
	std::map<std::string, double> m_scaleProductsOutput; //pre*post-scales 

	std::map<std::string, Algorithm*> prescalers; ///< map of prescaler algorithms

	mutable IPropertyConfigSvc * m_propertyConfigSvc;
	bool m_filter;			      //
	std::string m_propertyConfigSvcName;          ///< Name of PropertyConfigSvc
	Condition *m_condTrigger;                     ///< Condition for sampling coefficients
	std::string m_hltDecReportsLocation;	      ///< Location of the DecReports
	unsigned int m_lastTCK;			      //
	unsigned int m_tck;			      //
	unsigned int m_outputTCK;			      //
	std::string m_outRepLoc;

};
#endif // TURBOPRESCALER_H
