//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_Facade_h
#define depend_Facade_h

#include <string>
#include <memory>

namespace xml
{
    class xostream;
}

namespace depend
{
    class ModuleVisitor;
    class FileVisitor;
    class LineVisitor;
    class UncommentedLineVisitor;
    class IncludeVisitor;
    class ClassVisitor;
    class ClassMetric;
    class DependencyMetric;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade 
{
public:
    //! @name Constructors/Destructor
    //@{
             Facade();
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& path );
    void Serialize( xml::xostream& xos );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ModuleVisitor > moduleVisitor_;
    std::auto_ptr< FileVisitor > fileVisitor_;
    std::auto_ptr< LineVisitor > lineVisitor_;
    std::auto_ptr< UncommentedLineVisitor > uncommentedLineVisitor_;
    std::auto_ptr< IncludeVisitor > includeVisitor_;
    std::auto_ptr< ClassVisitor > classVisitor_;
    std::auto_ptr< ClassMetric > classMetric_;
    std::auto_ptr< DependencyMetric > dependencyMetric_;
    //@}
};

}

#endif // depend_Facade_h
