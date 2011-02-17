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
#include <map>
#include <vector>

namespace xml
{
    class xostream;
    class xisubstream;
    class xistream;
}

namespace depend
{
    class Finder_ABC;
    class ExternalModuleResolver_ABC;
    class InternalModuleResolver_ABC;
    class Log_ABC;
    class ModuleVisitor;
    class FileVisitor;
    class LineVisitor;
    class UncommentedLineVisitor;
    class IncludeVisitor;
    class DependencyMetric_ABC;
}

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Facade( xml::xisubstream xis );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void Visit( const std::string& path ) const;
    bool Process( xml::xisubstream xis ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< depend::Log_ABC > log_;
    std::auto_ptr< depend::Finder_ABC > finder_;
    std::auto_ptr< depend::ExternalModuleResolver_ABC > externalResolver_;
    std::auto_ptr< depend::ExternalModuleResolver_ABC > proxy_;
    std::auto_ptr< depend::ModuleVisitor > moduleVisitor_;
    std::auto_ptr< depend::FileVisitor > fileVisitor_;
    std::auto_ptr< depend::LineVisitor > lineVisitor_;
    std::auto_ptr< depend::UncommentedLineVisitor > uncommentedLineVisitor_;
    std::auto_ptr< depend::IncludeVisitor > includeVisitor_;
    std::auto_ptr< depend::InternalModuleResolver_ABC > internalResolver_;
    std::auto_ptr< depend::DependencyMetric_ABC > dependencyMetric_;
    //@}
};

#endif // depend_Facade_h
