
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

//-----------------------------------------------------------------------------
// boost variant/detail/substitute.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_DETAIL_SUBSTITUTE_HPP
#define BOOST_VARIANT_DETAIL_SUBSTITUTE_HPP

#include "boost/mpl/aux_/config/ctps.hpp"

#include "boost/variant/detail/substitute_fwd.hpp"
#include "boost/mpl/aux_/lambda_arity_param.hpp"
#include "boost/mpl/aux_/preprocessor/params.hpp"
#include "boost/mpl/aux_/preprocessor/repeat.hpp"
#include "boost/mpl/int_fwd.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/empty.hpp"
#include "boost/preprocessor/arithmetic/inc.hpp"
#include "boost/preprocessor/iterate.hpp"

namespace boost {
namespace detail { namespace variant {

#if !defined(BOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction substitute
//
// Substitutes one type for another in the given type expression.
//

//
// primary template
//
template <
      typename T, typename Dest, typename Source
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(
          typename Arity /* = ... (see substitute_fwd.hpp) */
        )
    >
struct substitute
{
    typedef T type;
};

//
// tag substitution specializations
//

#define BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(CV_) \
    template <typename Dest, typename Source> \
    struct substitute< \
          CV_ Source \
        , Dest \
        , Source \
          BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>) \
        > \
    { \
        typedef CV_ Dest type; \
    }; \
    /**/

BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG( BOOST_PP_EMPTY() )
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(const)
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(volatile)
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(const volatile)

#undef BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG

//
// pointer specializations
//
#define BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(CV_) \
    template <typename T, typename Dest, typename Source> \
    struct substitute< \
          T * CV_ \
        , Dest \
        , Source \
          BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>) \
        > \
    { \
        typedef typename substitute< \
              T, Dest, Source \
            >::type * CV_ type; \
    }; \
    /**/

BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER( BOOST_PP_EMPTY() )
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(const)
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(volatile)
BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(const volatile)

#undef BOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER

//
// reference specializations
//
template <typename T, typename Dest, typename Source>
struct substitute<
      T&
    , Dest
    , Source
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>)
    >
{
    typedef typename substitute<
          T, Dest, Source
        >::type & type;
};

//
// template expression (i.e., F<...>) specializations
//

#define BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL(N) \
    typedef typename substitute< \
          BOOST_PP_CAT(U,N), Dest, Source \
        >::type BOOST_PP_CAT(u,N); \
    /**/

#define BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF(z, N, _) \
    BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL( BOOST_PP_INC(N) ) \
    /**/

#define BOOST_PP_ITERATION_LIMITS (1,BOOST_MPL_METAFUNCTION_MAX_ARITY)
#define BOOST_PP_FILENAME_1 "boost/variant/detail/substitute.hpp"
#include BOOST_PP_ITERATE()

#undef BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL
#undef BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF

#endif // !defined(BOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

}} // namespace detail::variant
} // namespace boost

#endif // BOOST_VARIANT_DETAIL_SUBSTITUTE_HPP

///// iteration, depth == 1

#elif BOOST_PP_ITERATION_DEPTH() == 1
#define i BOOST_PP_FRAME_ITERATION(1)

template <
      template < BOOST_MPL_PP_PARAMS(i,typename P) > class T
    , BOOST_MPL_PP_PARAMS(i,typename U)
    , typename Dest
    , typename Source
    >
struct substitute<
      T< BOOST_MPL_PP_PARAMS(i,U) >
    , Dest
    , Source
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<( i )>)
    >
{
private:
    BOOST_MPL_PP_REPEAT(i, BOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF, _)

public:
    typedef T< BOOST_MPL_PP_PARAMS(i,u) > type;
};

#undef i
#endif // BOOST_PP_IS_ITERATING