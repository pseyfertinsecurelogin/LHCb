// ============================================================================
#ifndef LOKI_CONTEXT_H 
#define LOKI_CONTEXT_H 1
// ============================================================================
// Include files
// ============================================================================
// forward delcaration
// ============================================================================
class GaudiAlgorithm ; // from GaudiKernel
class IDVAlgorithm   ; // from DaVinciInterfaces
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class Context Context.h LoKi/Context.h
   *  Keep the context... 
   *  @author Vanya Belyaev
   *  @date   2018-08-10
   */
  class Context 
  {
  public:
    // =========================================================================
    /** constructor from algorithm pointers
     *  @param algo   own GaudiAlgorithm
     *  @param dvalgo own IDVAlgorithm
     */
    Context ( const GaudiAlgorithm* algo   = nullptr ,
              const IDVAlgorithm*   dvalgo = nullptr ) ;
    ///  destructor 
    ~Context() ;
    // ========================================================================
  public:
    // ========================================================================
    /// own GaudiAlgorithm 
    const GaudiAlgorithm*   algo () const { return   m_algo ; } 
    /// own IDVAlgorithm
    const IDVAlgorithm*   dvalgo () const { return m_dvalgo ; }
    // ========================================================================
  private:
    // ========================================================================
    /// own GaudiAlgorithm 
    const GaudiAlgorithm* m_algo   { nullptr } ; // own GaudiAlgorithm 
    /// own IDVAlgorithm
    const IDVAlgorithm*   m_dvalgo { nullptr } ; // own IDVAlgorithm
    // ========================================================================
  };
  // ==========================================================================
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_CONTEXT_H
// ============================================================================