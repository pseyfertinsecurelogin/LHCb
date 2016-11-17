#ifndef HLTCOMMON_HLTEVALUATOR_H
#define HLTCOMMON_HLTEVALUATOR_H 1

// Include files
#include <array>
#include <functional>

// boost
#include <boost/variant.hpp>

// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/GaudiException.h"
#include "DetDesc/Condition.h"

#include "Kernel/IHltMonitorSvc.h"

#include "LoKi/OdinTypes.h"
#include "LoKi/L0Types.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/IHltFactory.h"

/** @class HltEvaluator HltEvaluator.h
 *
 *
 *  @author Gerhard Raven
 *  @date   2008-07-29
 */
class HltEvaluator : public extends<GaudiHistoAlg, IIncidentListener> {
public:

   /// Standard constructor
   HltEvaluator( const std::string& name, ISvcLocator* pSvcLocator );

   StatusCode initialize() override;    ///< Algorithm initialization

protected:

   /// Decode
   virtual StatusCode decode() = 0;

   StatusCode times(double& t, double& w, double& et) const;

   template <class T>
   const T getData(const std::string location) {
      auto it = m_data.find(location);
      const DataObject* d{nullptr};
      if (it == end(m_data)) {
         d = getIfExists<T>(location);
         m_data.emplace(std::move(location), d);
      } else {
         d = it->second;
      }
      return static_cast<const T>(d);
   }

   enum { nBits = 3 * sizeof(unsigned int) * 8 };

   template<class PRED>
   struct Eval {
      Eval(std::string loc) : location{std::move(loc)} {}

      using Predicate = PRED;
      using Arg = typename PRED::argument_type;
      std::unique_ptr<PRED> predicate;
      StatEntity *counter = nullptr;
      AIDA::IHistogram1D* hist = nullptr;
      RateCounter* rate = nullptr;
      std::string location;
   };

   using ODINEval = Eval<LoKi::Types::ODIN_Cut>;
   using L0Eval = Eval<LoKi::Types::L0_Cut>;
   using HltEval = Eval<LoKi::Types::HLT_Cut>;
   using EvalVariant = boost::variant<ODINEval, L0Eval, HltEval>;

   class Builder : public boost::static_visitor<StatusCode> {
   public:

      Builder(HltEvaluator* parent, std::string expr,
              std::string title, std::string htitle)
         : m_parent{parent}, m_expr{std::move(expr)}, m_title{std::move(title)},
           m_htitle{std::move(htitle)} {}

      template <typename EVAL>
      StatusCode operator()( EVAL& eval ) {
         using BooleanConstant = typename LoKi::BasicFunctors<typename EVAL::Arg>::BooleanConstant;
         typename EVAL::Predicate cut(BooleanConstant(false));
         StatusCode sc = m_parent->factory().get(m_expr, cut, m_parent->preambulo());
         if (sc.isFailure()) return sc;

         eval.predicate.reset(cut.clone());
         eval.counter   = &(m_parent->counter(m_title));
         if (m_parent->m_hltMonSvc.isValid())
            eval.rate = &(m_parent->m_hltMonSvc->rateCounter(m_parent->name() + "/" + m_htitle));
         m_parent->declareInfo(boost::str( boost::format("COUNTER_TO_RATE[%s]") % m_htitle ),
                               *(eval.counter), m_htitle);
         int nBins = int(m_parent->m_timeSpan / m_parent->m_binWidth + 0.5);
         //TODO: set AxisLabels
         eval.hist = m_parent->book1D(m_htitle, 0, nBins * m_parent->m_binWidth, nBins);
         return StatusCode::SUCCESS;
      }

   private:

      HltEvaluator* m_parent;
      const std::string m_expr;
      const std::string m_title;
      const std::string m_htitle;

   };

   class Evaluator : public boost::static_visitor<bool> {
   public:

      Evaluator(HltEvaluator* parent, double t, double weight, double evtTime)
         : m_parent{parent}, m_t{t}, m_weight{weight}, m_evtTime{evtTime} {}

      template <typename EVAL>
      bool operator()( EVAL& eval ) {
         auto data = m_parent->getData<typename EVAL::Arg>(eval.location);
         if (!data) return false;
         if (!eval.predicate) return false;
         bool result = (*eval.predicate)(data);
         *(eval.counter) += result;
         if (result) eval.hist->fill(m_t, m_weight);
         if (result && eval.rate) eval.rate->count(m_evtTime);
         return result;
      }

   private:

      HltEvaluator* m_parent;
      double m_t;
      double m_weight;
      double m_evtTime;

   };

   struct Deleter : public boost::static_visitor<> {
      template <typename EVAL>
      void operator()( EVAL& eval ) const {
         eval.predicate.reset();
         eval.counter = nullptr;
         eval.hist = nullptr;
         eval.rate = nullptr;
      }
   };

   unsigned long long startOfRun() const {
      // just in case we did not have the conditions DB start-of-run time...
      if (m_startOfRun == 0) {
         auto odin = get<LHCb::ODIN>(m_odin_location);
         m_startOfRun = odin->gpsTime();
      }
      return m_startOfRun;

   }

   LoKi::Hybrid::IHltFactory& factory() const {
      if (!m_factory) {
         std::string factoryType = (0 == name().find ( "Hlt1" ) ?
                                    "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC" :
                                    (0 == name().find ( "Hlt2" ) ?
                                     "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC" :
                                     "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC"));
         m_factory = tool<LoKi::Hybrid::IHltFactory>(factoryType);
      }
      return *m_factory;
   }

   const std::string& preambulo() const {
      return m_preambulo;
   }

   double binWidth() const { return m_binWidth; }
   double timeSpan() const { return m_timeSpan; }

   SmartIF<IHltMonitorSvc> m_hltMonSvc;
   mutable LoKi::Hybrid::IHltFactory* m_factory = nullptr;

   std::string m_odin_location;
   std::string m_l0_location;
   std::string m_hlt_location[2];
   std::string m_raw_location;
   std::map<unsigned int,std::string> m_bits;

   bool m_evals_updated      = false;
   bool m_preambulo_updated  = false;
   std::string m_preambulo;

private:

   bool m_useCondDB;
   std::string m_monSvc;

   std::vector<std::string> m_preambulo_ ;             // the preambulo property

   std::unordered_map<std::string, const DataObject*> m_data;
   Condition *m_runpars = nullptr;
   SmartIF<IUpdateManagerSvc> m_updMgrSvc;

   mutable unsigned long long m_startOfRun = 0;
   double m_binWidth = 10;   // in _seconds_!
   double m_timeSpan = 4000; // in _seconds_!

   StatusCode i_updateConditions();
   void handle(const Incident&) override;
   void updatePreambulo ( Property& );

};
#endif // HLTEVALUATOR_H
