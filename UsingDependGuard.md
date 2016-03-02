# Introduction #

**Depend Guard** is a tool made for automatic dependency check on a big project with many _new_ developpers. Architects can congeal a part of their dependency design and forbid dependencies between modules. Any unwanted dependency will produce an error with the incriminated include.

# Command line parameters #

`Usage: depend_guard --dependencies=input.xml --graph=graph.xml`

  * **help**(-h): produce help message
  * **version**(-v): produce version message
  * **dependencies**: set input xml file for dependency guard check
  * **graph**: set graph input file (from [depend](UsingDepend.md)) for guard analysis

Example of dependency XML input:
```
<dependencies>
    <module name='depend_app'>
        <dependency>depend</dependency>
    </module>
    <module name='depend_guard_app'>
        <dependency>depend</dependency>
    </module>
    <module name='depend_metrics_app'>
        <dependency>depend</dependency>
    </module>
    <module name='depend_components_app'>
        <dependency>depend</dependency>
    </module>
    <module name='depend_graph_app'>
        <dependency>depend</dependency>
    </module>
</dependencies>
```

# Dependency checking #

There are four cases when checking dependencies:

  * **Authorized dependency**: A dependency from a source module to a target module defined in the xml and checked during code analysis. Nothing more append.
```
<dependencies>
    <module name='source'>
        <dependency>target</dependency>
    </module>
</dependencies>
```

  * **Unexisting dependency**: A dependency declared in the XML but not checked during code analysis. A warning is produced to prevent that there is no dependency any more.
`Warning: no dependency found from module 'source' to module 'target', you probably should remove it from your dependency file.`

  * **Obsolete dependency**: A dependency declared but labelled as **obsolete-dependency** and checked during code analysis. A warning is produced with the incriming include. The idea is to let the architech defines **goals** for dependency cleaning. The warning will be produced until the dependency removal. When the dependency is effectively removed, the warning is replaced by the second case **Unexisting dependency**. The obsolete dependency can then be cleaned from dependency file.

`root/src/path/source/Class.cpp(23): warning: obsolete dependency has been found from module 'source' to module 'target'.`

```
<dependencies>
    <module name='source'>
        <obsolete-dependency>target</obsolete-dependency>
    </module>
</dependencies>
```

  * **Forbidden dependency**: A dependency between two modules discovered during code analysis but not declared as an authorized dependency in the XML. An error is produced with the incrimining include. All undeclared dependencies are **implicitly** forbidden dependencies.
`root/src/path/source/Class.cpp(23): error : dependency from module 'source' to module 'target' is forbidden.`