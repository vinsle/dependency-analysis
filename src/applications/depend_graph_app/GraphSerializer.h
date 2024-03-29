//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_graph_app_GraphSerializer_h
#define depend_graph_app_GraphSerializer_h

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace xml
{
    class xisubstream;
}

// =============================================================================
/** @class  GraphSerializer
    @brief  Graph serializer
*/
// Created: SLI 2010-08-27
// =============================================================================
class GraphSerializer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GraphSerializer( xml::xisubstream xis );
    virtual ~GraphSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const std::string& dot, const std::string& filename ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Options;
    //@}

private:
    //! @name Member data
    //@{
    const std::string layout_;
    const std::string format_;
    const T_Options graph_;
    const T_Options node_;
    const T_Options edge_;
    //@}
};

#endif // depend_graph_app_GraphSerializer_h
