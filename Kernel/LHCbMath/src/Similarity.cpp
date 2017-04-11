
#include "LHCbMath/Similarity.h"
#include "LHCbMath/CPUDispatch.h"
#include <type_traits>

namespace {
  template <typename Vtbl, typename Trampoline, typename... Args>
  decltype(auto) dispatch_fn( const Vtbl& vtbl, Trampoline& t, Args&&... args )
  {
    t = LHCb::CPU::dispatch(vtbl);
    return (*t)(std::forward<Args>(args)...);
  }
}

namespace LHCb {
  namespace Math {

    // TODO: replace by (static?) functor (instance)s...
    //       (maybe templated, with multiple explicit specializations & tag dispatch???)
    //
    //       or can we replace the 'explicit' vtbl with inheritance? After all,
    //       this is 'just' runtime-polymorphism... but then the 'global'
    //       instance in the trampoline functions should be updated somehow.
    //       That way, they get all shunted simultaneously on the first invocation
    //       of any individual one...
    //
    namespace avx2 {
      extern void similarity_5_1(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_5(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_7(const double* Ci, const double* Fi, double* Ti);
      extern bool average(const double* X1, const double* C1,
                          const double* X2, const double* C2,
                          double *X, double *C );
      extern double filter( double* X, double* C,
                            const double* Xref, const double* H,
                            double refResidual, double errorMeas2 );
    }
    namespace avx {
      extern void similarity_5_1(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_5(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_7(const double* Ci, const double* Fi, double* Ti);
      extern bool average(const double* X1, const double* C1,
                          const double* X2, const double* C2,
                          double *X, double *C );
      extern double filter( double* X, double* C,
                            const double* Xref, const double* H,
                            double refResidual, double errorMeas2 );
    }
    namespace sse3 {
      extern void similarity_5_1(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_5(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_7(const double* Ci, const double* Fi, double* Ti);
    }
    namespace generic {
      extern void similarity_5_1(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_5(const double* Ci, const double* Fi, double* Ti);
      extern void similarity_5_7(const double* Ci, const double* Fi, double* Ti);
      extern bool average(const double* X1, const double* C1,
                          const double* X2, const double* C2,
                          double *X, double *C );
      extern double filter( double* X, double* C,
                            const double* Xref, const double* H,
                            double refResidual, double errorMeas2 );
    }


    namespace dispatch {

      void similarity_5_1(const double* Ci, const double* Fi, double* ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_1 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_1 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_1 ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_1 ) };
        dispatch_fn( vtbl, LHCb::Math::similarity_5_1, Ci, Fi, ti );
      }

      void similarity_5_5(const double* Ci, const double* Fi, double* ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_5 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_5 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_5  ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_5 ) };
        dispatch_fn( vtbl, LHCb::Math::similarity_5_5, Ci, Fi, ti );
      }

      void similarity_5_7(const double* Ci, const double* Fi, double* ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_7 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_7 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_7 ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_7 ) };
        dispatch_fn( vtbl, LHCb::Math::similarity_5_7, Ci, Fi, ti );
      }

      bool average(const double* X1, const double* C1,
                   const double* X2, const double* C2,
                   double *X, double *C )
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::average ),
                      std::make_pair( CPU::AVX, avx::average ),
                      std::make_pair( CPU::GENERIC, generic::average ) };
        return dispatch_fn( vtbl, LHCb::Math::average, X1, C1, X2, C2, X, C );
      }

      double filter(double* X, double* C,
                    const double* Xref, const double* H,
                    double refResidual, double errorMeas2 ) {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::filter ),
                      std::make_pair( CPU::AVX, avx::filter ),
                      std::make_pair( CPU::GENERIC, generic::filter ) };
        return dispatch_fn( vtbl, LHCb::Math::filter, X, C, Xref, H, refResidual, errorMeas2 );
      }

    }

    similarity_t similarity_5_1 = &dispatch::similarity_5_1;
    similarity_t similarity_5_5 = &dispatch::similarity_5_5;
    similarity_t similarity_5_7 = &dispatch::similarity_5_7;

    average_t average = &dispatch::average;
    filter_t filter = &dispatch::filter;

  } 
}
