#ifndef FAST_MATH_ACCURACY_TAGS
#define FAST_MATH_ACCURACY_TAGS

namespace LHCb
{
  namespace Math
  {
    //------------------------------------------------------------------------------
    /**
     * @brief Base class to all accuracy requirement tags
     *
     * Accuracy requirement tags are used to relax the accuracy requirement of a function in order to be faster.
     * See the documentation of the function to know what are the accuracy guarantees.
     *
     * The default accuracy requirement is @ref accurate
     */
    struct accuracy_tag {};

    /**
     * @brief As accurate as efficiently possible.
     *
     * This is the default accuracy requirement.
     * It is designed to be at least as accurate as @ref ieee while being at least as fast as @ref ieee.
     *
     * @note
     * Currently, results may differ between @ref accurate and @ref ieee only on KNL
     * where `_mm_rsqrt28_ps` can compute 1/sqrt(x) with all bits correct;
     * and on ARM32 where there is no Neon instruction to divide or to take a square root.
     */
    struct accurate : accuracy_tag {};

    /**
     * @brief strict IEEE754 compliance
     *
     * This accuracy requirement should be used when you want reproducible results
     * accross compilers, machines, and architectures.
     *
     * @warning
     * On ARM 32bits and PowerPC 32bits, IEEE754 cannot be achieved with SIMD instructions
     * Thus, a fallback using scalar instructions is used on these architectures
     */
    struct ieee : accuracy_tag {};

    /**
     * @brief Relaxed strict accuracy constraint for fast execution
     *
     * This accuracy requirement should give an error less than 2 bits.
     * This usually enables faster execution than @ref accurate.
     *
     * @note
     * In some cases, the @ref fast implementation is the same as the @ref accurate one.
     * In those cases, no timing or accuracy differences could be observed.
     */
    struct fast : accuracy_tag {};

    /**
     * @brief Faster execution, good accuracy
     *
     * This accuracy requirement usually gives good results.
     * It is guaranted to be at least as fast as @ref fast.
     *
     * @note
     * It might use the same implementation as @ref fast if making it faster would give too bad accuracy.
     *
     * @warning
     * The full range of the double precision floating point is not covered with some implementations.
     * Use this accuracy requirement only if the input fits within the range of a single precision `float` (abs in [10^-38, 10^38]).
     * Otherwise, this is undefined behaviour.
     */
    struct faster : accuracy_tag {};

    /**
     * @brief Rough estimation, super fast
     *
     * No constraint on accuracy. It usually results in very rough estimation, but is super fast.
     *
     * @warning
     * The full range of the double precision floating point is not covered with some implementations.
     * Use this accuracy requirement only if the input fits within the range of a single precision `float` (abs in [10^-38, 10^38]).
     * Otherwise, this is undefined behaviour.
     */
    struct fastest : accuracy_tag {};

  }
}

#endif
