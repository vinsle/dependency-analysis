//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Facade_h
#define depend_Facade_h

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>
#include <vector>

namespace xml
{
    class xostream;
}

namespace depend
{
    class Filter_ABC;
    class ModuleVisitor;
    class FileVisitor;
    class LineVisitor;
    class UncommentedLineVisitor;
    class IncludeVisitor;
    class ClassVisitor;
    class ClassMetric;
    class DependencyMetric;
    class ModuleSerializer;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::string > T_Filter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Facade( const T_Filter& filter );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& path );
    void Serialize( xml::xostream& xos );
    void Serialize( std::ostream& os );
    void Serialize( const std::string& filename );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Filter_ABC > filter_;
    std::auto_ptr< ModuleVisitor > moduleVisitor_;
    std::auto_ptr< FileVisitor > fileVisitor_;
    std::auto_ptr< LineVisitor > lineVisitor_;
    std::auto_ptr< UncommentedLineVisitor > uncommentedLineVisitor_;
    std::auto_ptr< IncludeVisitor > includeVisitor_;
    std::auto_ptr< ClassVisitor > classVisitor_;
    std::auto_ptr< ClassMetric > classMetric_;
    std::auto_ptr< DependencyMetric > dependencyMetric_;
    std::auto_ptr< ModuleSerializer > moduleSerializer_;
    //@}
};

}

#endif // depend_Facade_h
