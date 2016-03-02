# Introduction #

Depend application is the first step in your code analysis. It parses all your C++ code base, identifying dependencies (includes) between internal (yours) and external modules.

Depend's main goal is to produce an XML file containing the code analysis result. The other tools in the suite uses this XML to compute their own analysis.

But depend can also be used to detect include errors like case sensitive includes, "" vs <> includes, etc...

See UnderstandingDependencyDetection for more informations about directories structure, module definition, dependency detection and limitations.

# Command line parameters #

`Usage: depend [options] path1 path2...`

  * **help**(-h): produce help message
  * **version**(-v): produce version message
  * **path**: add a directory containing modules for analysis
  * **output**: set output file
  * **include**(-I): add an include directory path for external dependency (dependency name can be forced with following syntax: --include="directory**,name**")
  * **exclude**(-E): add an include directory path excluded from the dependencies and warnings
  * **warning**: enable warnings

# Exemples #

A simple invocation on dependency-analysis sources:

`depend.exe --output=depend.xml --path=src/libraries --path=src/applications --include=include --exclude="$VSINSTALLDIR/VC/include" --warning`

It produces an xml like this:
```
<graph>
  <nodes>
    <node name="depend">
      <efferent-dependencies/>
      <external-dependencies>
        <dependency name="boost">
          <context>dependency-analysis/trunk/src/libraries/depend/ExternalModuleResolver.cpp(16)</context>
          <context>dependency-analysis/trunk/src/libraries/depend/FileVisitor.cpp(15)</context>
          ...
        </dependency>
        ...
      </external-dependencies>
    </node>
  </nodes>
  <classes>
    <node name="depend">
      <class abstract="false" context="dependency-analysis/trunk/src/libraries/depend/ClassDependencyMetric.h(29)" name="ClassDependencyMetric"/>
      <class abstract="false" context="dependency-analysis/trunk/src/libraries/depend/ClassLoader.h(31)" name="ClassLoader"/>
       ...
    </node>
    ...
  </classes>
</graph>
```