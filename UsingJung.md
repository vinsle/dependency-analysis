# Using Jung #

A futur way is to use [API](http://jung.sourceforge.net/doc/api/index.html) as a front-end for "dynamic" analysis.

It may replace Graphviz as well for "static" analysis ( [how to dump a graph in a jpg](http://jung.sourceforge.net/doc/JUNGVisualizationGuide.html) ).

Jung has a [GraphML](http://graphml.graphdrawing.org/) parser as well.

Architecture may be a back-end in C++ (the same as now, without its renderer part) producing a GraphML file, sent to a front-end in java/jung.