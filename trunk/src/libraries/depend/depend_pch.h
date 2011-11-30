//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_pch_h
#define depend_pch_h

#ifdef _MSC_VER
#   pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#   pragma warning( disable : 4503 ) // decorated name length exceeded, name was truncated
#endif

#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4701 4702 4996 )
#elif defined __GNUC__
#   pragma GCC system_header
#   include <bits/stl_algo.h>
#endif
#include <boost/xpressive/xpressive.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

#endif // depend_pch_h
