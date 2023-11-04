#ifndef SIMATH_PLATFORM_H_
#define SIMATH_PLATFORM_H_

/* first we define some macros to help with alignment across different compilers */

#if defined ( _MSC_VER )    // microsoft compilers
#define SIM_ALIGN_16 __declspec(align(16))
#define SIM_ALIGN_32 __declspec(align(32))
#define SIM_FORCEINLINE __forceinline
#elif defined( __GNUC__ ) || defined( __clang__ )
#define SIM_ALIGN_16 __attribute__((aligned(16)))
#define SIM_ALIGN_32 __attribute__((aligned(32)))
#define SIM_FORCEINLINE __attribute__((always_inline))
#else
#error "Unsupported compiler (simath)"
#endif

/* AMD x86_64 */
#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) ) && ! defined(__x86_64__)
#define __x86_64__ 1
#endif

/* selection of instruction set level */
#ifndef SIM_INSTRSET        // INSTRUCTION SET
#if defined ( __AVX512VL__ ) && defined ( __AVX512BW__ ) && defined ( __AVX512DQ__ )
#define SIM_INSTRSET 10
#elif defined ( __AVX512F__ ) || defined ( __AVX512__ )
#define SIM_INSTRSET 9
#elif defined ( __AVX2__ )
#define SIM_INSTRSET 8
#elif defined ( __AVX__ )
#define SIM_INSTRSET 7
#elif defined ( __SSE4_2__ )
#define SIM_INSTRSET 6
#elif defined ( __SSE4_1__ )
#define SIM_INSTRSET 5
#elif defined ( __SSSE3__ )
#define SIM_INSTRSET 4
#elif defined ( __SSE3__ )
#define SIM_INSTRSET 3
#elif defined ( __SSE2__ ) || defined ( __x86_64__ )
#define SIM_INSTRSET 2
#elif defined ( __SSE__ )
#define SIM_INSTRSET 1
#elif defined ( _M_IX86_FP )           // Weird MS compiler definition of SSE1 and SSE2
#define SIM_INSTRSET _M_IX86_FP
#else
#error "Unable to compile due to unsupported instruction set (smgl)."
#endif // instruction set defines
#endif // SMGL_INSTRSET

/* including the appropriate headers */
#if SIM_INSTRSET > 7                       // AVX2 and later
#if defined (__GNUC__) && ! defined (__INTEL_COMPILER)
#include <x86intrin.h>
#else
#include <immintrin.h>                 // MS/Intel version of immintrin.h covers AVX and later
#endif // __GNUC__
#elif SIM_INSTRSET == 7
#include <immintrin.h>                 // AVX
#elif SIM_INSTRSET == 6
#include <nmmintrin.h>                 // SSE4.2
#elif SIM_INSTRSET == 5
#include <smmintrin.h>                 // SSE4.1
#elif SIM_INSTRSET == 4
#include <tmmintrin.h>                 // SSSE3
#elif SIM_INSTRSET == 3
#include <pmmintrin.h>                 // SSE3
#elif SIM_INSTRSET == 2
#include <emmintrin.h>                 // SSE2
#elif SIM_INSTRSET == 1
#include <xmmintrin.h>                 // SSE
#endif // INSTRSET


#if defined (__XOP__) || defined (__FMA4__)
#ifdef __GNUC__
#include <x86intrin.h>                 // AMD XOP (Gnu)
#else
#include <ammintrin.h>                 // AMD XOP (Microsoft)
#endif //  __GNUC__
#elif defined (__SSE4A__)              // AMD SSE4A
#include <ammintrin.h>
#endif

// Debug assertions
#include <assert.h>
#if defined SIM_DEBUG
#define SIM_ASSERT(x) assert((x))
#else
#define SIM_ASSERT(x)
#endif

#endif // SIMATH_PLATFORM_H_
