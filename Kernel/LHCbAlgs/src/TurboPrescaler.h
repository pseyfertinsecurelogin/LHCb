#ifndef TURBOPRESCALER_H 
#define TURBOPRESCALER_H 1

// Include files
// from DaVinci, this is a specialized GaudiAlgorithm
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/HltDecReports.h"

/** @class TurboPrescaler TurboPrescaler.h
 *  
 *
 *  @author Sean Benson
 *  @date   2016-06-13
 */
class TurboPrescaler : public GaudiAlgorithm {
    public: 
	TurboPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

	virtual StatusCode initialize();    
	virtual StatusCode execute   ();    

    protected:

    private:
    	std::string m_scalerName; 
	std::string m_postScalerName; 
	std::string m_preScalerName; 
	
	void getPrescalesFromTCK(unsigned int, std::map<std::string, double> &, std::map<std::string, double> &, std::map<std::string, double> &); 
	bool endedWith(const std::string &lineName, const std::string &ending);

	void setupPrescalers(); 
	void updatePrescalers(); 

	std::map<std::string, double> m_prescalesInput; 
	std::map<std::string, double> m_postscalesInput; 
	std::map<std::string, double> m_scaleProductsInput; 

	std::map<std::string, Algorithm*> prescalers; 

	mutable IPropertyConfigSvc * m_propertyConfigSvc=nullptr;
	bool m_filter=false;			      
	std::string m_propertyConfigSvcName;        
	std::string m_hltDecReportsLocation;	      
	unsigned int m_lastTCK=0;			      
	unsigned int m_outputTCK=0;			      
        std::map<std::string,double> m_outputPS;			      
	std::string m_outRepLoc;

};
#endif // TURBOPRESCALER_H
