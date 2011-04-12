//
//  Copyright Silvin Lubecki 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef depend_graph_app_Facade_h
#define depend_graph_app_Facade_h

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "depend/UnitObserver_ABC.h"

namespace xml
{
    class xostream;
    class xisubstream;
    class xistream;
}

namespace depend
{
    class ClassLoader;
    class Filter_ABC;
    class UnitSerializer;
    class ClassMetricVisitor_ABC;
    class DependencyVisitor_ABC;
    template< typename T > class Visitable;
}

class GraphSerializer;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SLI 2010-08-18
// =============================================================================
class Facade : private depend::UnitObserver_ABC
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
    //! @name Operations
    //@{
    virtual void NotifyUnit( const std::string& unit, const std::string& context );
    //@}

private:
    //! @name Helpers
    //@{
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
    std::auto_ptr< depend::Filter_ABC > filter_;
    std::auto_ptr< depend::ClassLoader > classLoader_;
    std::auto_ptr< depend::Visitable< depend::UnitObserver_ABC > > unitCache_;
    std::auto_ptr< depend::Visitable< depend::ClassMetricVisitor_ABC > > classMetric_;
    std::auto_ptr< depend::Visitable< depend::DependencyVisitor_ABC > > dependencyMetric_;
    std::auto_ptr< depend::UnitSerializer > unitSerializer_;
    std::auto_ptr< GraphSerializer > graphSerializer_;
    std::vector< std::string > modules_;
    //@}
};

#endif // depend_graph_app_Facade_h
