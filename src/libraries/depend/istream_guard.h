//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_istream_guard_h
#define depend_istream_guard_h

#include <fstream>
#include <boost/noncopyable.hpp>

namespace depend
{
// =============================================================================
/** @class  istream_guard
    @brief  i stream guard
*/
// Created: SLI 2010-08-17
// =============================================================================
class istream_guard : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit istream_guard( std::istream& stream )
        : stream_  ( stream )
        , position_( stream.tellg() )
        , state_   ( stream.rdstate() )
    {
        // NOTHING
    }
    virtual ~istream_guard()
    {
        stream_.clear( state_ );
        stream_.seekg( position_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::istream& stream_;
    std::fstream::pos_type position_;
    std::ios_base::iostate state_;
    //@}
};

}

#endif // depend_istream_guard_h
