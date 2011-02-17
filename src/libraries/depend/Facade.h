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
    class ClassMetric_ABC;
    class DependencyMetric_ABC;
    class UnitSerializer;
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
    std::auto_ptr< Log_ABC > log_;
    std::auto_ptr< Filter_ABC > filter_;
    std::auto_ptr< Finder_ABC > finder_;
    std::auto_ptr< ExternalModuleResolver_ABC > externalResolver_;
    std::auto_ptr< ExternalModuleResolver_ABC > proxy_;
    std::auto_ptr< ModuleVisitor > moduleVisitor_;
    std::auto_ptr< FileVisitor > fileVisitor_;
    std::auto_ptr< LineVisitor > lineVisitor_;
    std::auto_ptr< UncommentedLineVisitor > uncommentedLineVisitor_;
    std::auto_ptr< IncludeVisitor > includeVisitor_;
    std::auto_ptr< ClassVisitor > classVisitor_;
    std::auto_ptr< ClassMetric_ABC > classMetric_;
    std::auto_ptr< InternalModuleResolver_ABC > internalResolver_;
    std::auto_ptr< DependencyMetric_ABC > dependencyMetric_;
    std::auto_ptr< UnitSerializer > unitSerializer_;
    std::auto_ptr< GraphSerializer > graphSerializer_;
    std::vector< std::string > modules_;
    //@}
};

}

#endif // depend_Facade_h
