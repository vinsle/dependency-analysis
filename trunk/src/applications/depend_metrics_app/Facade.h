//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_metrics_app_Facade_h
#define depend_metrics_app_Facade_h

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
    class Filter_ABC;
    class Finder_ABC;
    class ExternalModuleResolver_ABC;
    class InternalModuleResolver_ABC;
    class Log_ABC;
    class ModuleVisitor;
    class FileVisitor;
    class LineVisitor;
    class UncommentedLineVisitor;
    class IncludeVisitor;
    class ClassVisitor;
    class UnitSerializer;
    class ClassMetricVisitor_ABC;
    class DependencyMetricVisitor_ABC;
    template< typename T > class Visitable;
}

class GraphSerializer;

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

public:
    //! @name Operations
    //@{
    void Process( xml::xisubstream xis );
    //@}

private:
    //! @name Helpers
    //@{
    void Visit( xml::xistream& xis );
    void Serialize( xml::xistream& xis );
    void Serialize( xml::xostream& xos );
    void Serialize( std::ostream& os );
    void Serialize( const std::string& filename );
    void SerializeAll( const std::string& filename );
    //@}

private:
    //! @name Member data
    //@{
    const std::string option_;
    bool extend_;
    std::auto_ptr< depend::Log_ABC > log_;
    std::auto_ptr< depend::Filter_ABC > filter_;
    std::auto_ptr< depend::Finder_ABC > finder_;
    std::auto_ptr< depend::ExternalModuleResolver_ABC > externalResolver_;
    std::auto_ptr< depend::ExternalModuleResolver_ABC > proxy_;
    std::auto_ptr< depend::ModuleVisitor > moduleVisitor_;
    std::auto_ptr< depend::FileVisitor > fileVisitor_;
    std::auto_ptr< depend::LineVisitor > lineVisitor_;
    std::auto_ptr< depend::UncommentedLineVisitor > uncommentedLineVisitor_;
    std::auto_ptr< depend::IncludeVisitor > includeVisitor_;
    std::auto_ptr< depend::ClassVisitor > classVisitor_;
    std::auto_ptr< depend::Visitable< depend::ClassMetricVisitor_ABC > > classMetric_;
    std::auto_ptr< depend::InternalModuleResolver_ABC > internalResolver_;
    std::auto_ptr< depend::Visitable< depend::DependencyMetricVisitor_ABC > > dependencyMetric_;
    std::auto_ptr< depend::UnitSerializer > unitSerializer_;
    std::auto_ptr< GraphSerializer > graphSerializer_;
    std::vector< std::string > modules_;
    //@}
};

#endif // depend_metrics_app_Facade_h
