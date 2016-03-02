# First graph #

Depend application is the first step in your code analysis.
It parses all your C++ code base, identifying dependencies (includes) between
internal (yours) and external modules.

A simple invocation on dependency-analysis sources (assuming you already checkouted dependency-analysis sources):

`./depend --output=depend.xml --path=src/libraries --path=src/applications --warning`

Depend Graph is a tool made for graph generation using GraphViz.
Each module is represented as a node and an edge represents a module dependency to another module.
GraphViz must be installed before using Depend Graph.

`./depend_graph --output=graph.png --input=depend.xml`