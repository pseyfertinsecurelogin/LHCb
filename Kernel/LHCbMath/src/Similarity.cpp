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
     namespace detail {

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
      extern void similarity_5_1(gsl::span<const double,15> Ci, gsl::span<const double, 5> Fi, gsl::span<double, 1> Ti);
      extern void similarity_5_5(gsl::span<const double,15> Ci, gsl::span<const double,25> Fi, gsl::span<double,15> Ti);
      extern void similarity_5_7(gsl::span<const double,15> Ci, gsl::span<const double,35> Fi, gsl::span<double,28> Ti);
      extern bool average(gsl::span<const double,5> X1, gsl::span<const double,15> C1,
                          gsl::span<const double,5> X2, gsl::span<const double,15> C2,
                          gsl::span<double,5> X, gsl::span<double,15> C );
      extern double filter( span<double,5> X, span<double,15> C,
                            span<const double,5> Xref, span<const double,5> H,
                            double refResidual, double errorMeas2 );
    }
    namespace avx {
      extern void similarity_5_1(gsl::span<const double,15> Ci, gsl::span<const double, 5> Fi, gsl::span<double, 1> Ti);
      extern void similarity_5_5(gsl::span<const double,15> Ci, gsl::span<const double,25> Fi, gsl::span<double,15> Ti);
      extern void similarity_5_7(gsl::span<const double,15> Ci, gsl::span<const double,35> Fi, gsl::span<double,28> Ti);
      extern bool average(gsl::span<const double,5> X1, gsl::span<const double,15> C1,
                          gsl::span<const double,5> X2, gsl::span<const double,15> C2,
                          gsl::span<double,5> X, gsl::span<double,15> C );
      extern double filter( span<double,5> X, span<double,15> C,
                            span<const double,5> Xref, span<const double,5> H,
                            double refResidual, double errorMeas2 );
    }
    namespace sse3 {
      extern void similarity_5_1(gsl::span<const double,15> Ci, gsl::span<const double, 5> Fi, gsl::span<double, 1> Ti);
      extern void similarity_5_5(gsl::span<const double,15> Ci, gsl::span<const double,25> Fi, gsl::span<double,15> Ti);
      extern void similarity_5_7(gsl::span<const double,15> Ci, gsl::span<const double,35> Fi, gsl::span<double,28> Ti);
    }
    namespace generic {
      extern void similarity_5_1(gsl::span<const double,15> Ci, gsl::span<const double, 5> Fi, gsl::span<double, 1> Ti);
      extern void similarity_5_5(gsl::span<const double,15> Ci, gsl::span<const double,25> Fi, gsl::span<double,15> Ti);
      extern void similarity_5_7(gsl::span<const double,15> Ci, gsl::span<const double,35> Fi, gsl::span<double,28> Ti);
      extern bool average(gsl::span<const double,5> X1, gsl::span<const double,15> C1,
                          gsl::span<const double,5> X2, gsl::span<const double,15> C2,
                          gsl::span<double,5> X, gsl::span<double,15> C );
      extern double filter( span<double,5> X, span<double,15> C,
                            span<const double,5> Xref, span<const double,5> H,
                            double refResidual, double errorMeas2 );
    }


    namespace dispatch {

      void similarity_5_1(gsl::span<const double,15> Ci, gsl::span<const double, 5> Fi, gsl::span<double,1> ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_1 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_1 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_1 ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_1 ) };
        dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_1, Ci, Fi, ti );
      }

      void similarity_5_5(gsl::span<const double,15> Ci, gsl::span<const double,25> Fi, gsl::span<double,15> ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_5 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_5 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_5  ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_5 ) };
        dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_5, Ci, Fi, ti );
      }

      void similarity_5_7(gsl::span<const double,15> Ci, gsl::span<const double,35> Fi, gsl::span<double,28> ti)
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::similarity_5_7 ),
                      std::make_pair( CPU::AVX, avx::similarity_5_7 ),
                      std::make_pair( CPU::SSE3, sse3::similarity_5_7 ),
                      std::make_pair( CPU::GENERIC, generic::similarity_5_7 ) };
        dispatch_fn( vtbl, LHCb::Math::detail::similarity_5_7, Ci, Fi, ti );
      }

      bool average(gsl::span<const double,5> X1, gsl::span<const double,15> C1,
                   gsl::span<const double,5> X2, gsl::span<const double,15> C2,
                   gsl::span<double,5> X, gsl::span<double,15> C )
      {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::average ),
                      std::make_pair( CPU::AVX, avx::average ),
                      std::make_pair( CPU::GENERIC, generic::average ) };
        return dispatch_fn( vtbl, LHCb::Math::detail::average, X1, C1, X2, C2, X, C );
      }

      double filter( span<double,5> X, span<double,15> C,
                     span<const double,5> Xref, span<const double,5> H,
                     double refResidual, double errorMeas2 ) {
        auto vtbl = { std::make_pair( CPU::AVX2, avx2::filter ),
                      std::make_pair( CPU::AVX, avx::filter ),
                      std::make_pair( CPU::GENERIC, generic::filter ) };
        return dispatch_fn( vtbl, LHCb::Math::detail::filter, X, C, Xref, H, refResidual, errorMeas2 );
      }

    }

    similarity_t<5,1> similarity_5_1 = &dispatch::similarity_5_1;
    similarity_t<5,5> similarity_5_5 = &dispatch::similarity_5_5;
    similarity_t<5,7> similarity_5_7 = &dispatch::similarity_5_7;

    average_t average = &dispatch::average;
    filter_t filter = &dispatch::filter;

    }
  }
}
