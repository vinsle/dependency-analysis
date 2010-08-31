//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_GraphSerializer_h
#define depend_GraphSerializer_h

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace depend
{
// =============================================================================
/** @class  GraphSerializer
    @brief  Graph serializer
*/
// Created: SLI 2010-08-27
// =============================================================================
class GraphSerializer : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Options;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GraphSerializer( const std::string& layout, const std::string& format,
                            const T_Options& graph, const T_Options& node, const T_Options& edge );
    virtual ~GraphSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const std::string& dot, const std::string& filename ) const;
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

}

#endif // depend_GraphSerializer_h
