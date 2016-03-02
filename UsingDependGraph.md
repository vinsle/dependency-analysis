<font color='red'>/!\ <b>Important</b> /!\</font>: Graphviz <font color='red'><b>must</b></font> be installed before using **Depend Graph**.

# Introduction #

**Depend Graph** is a tool made for graph generation using [GraphViz](http://www.graphviz.org). Each module is represented as a node and an edge represents a module dependency to another module.

Three metrics are actually drawn:
  * **I** for Instability
  * **A** for Abstractness
  * **D** for Distance (see UnderstandingMetrics for more informations)
Node coloration depends on distance value, from <font color='red'><b>Red</b></font> with D = 100 through <font color='orange'><b>Orange</b></font> with D = 50, to <font color='green'><b>Green</b></font> with D = 0 (_the bigger the Distance, the poorer the design_).

All strongly connected components (or [circular dependencies](UsingDependComponents.md)) are represented with the same color on edge.

Default Graphviz layout can be totally overridden using proper command line parameters (**layout**, **graph**, **node**, **edge**).

# Command line parameters #

`Usage: depend_graph [options] --output=output --input=graph.xml`

## Generic options ##
  * **help**(-h): produce help message
  * **version**(-v): produce version message
  * **input**: set graph input file for analysis
  * **output**: set graph output file
  * **filter**: select only modules in filter and their afferent and efferent modules
  * **extend**: extend to all reachable modules if filter is enabled
  * **all**: render every graph centered on each node"

See UnderstandingFilters for more informations on filter and extend options.

## Graph options ##
  * **layout**: set layout algorithm (dot or neato, dot by default)
  * **format**: set graph output format (png, jpg, svg or pdf, png by default)
  * **graph**(-g): set graph options
  * **node**(-n): set node options
  * **edge**(-e): set edge options
  * **dependencies**: set optional external dependencies drawing (values are: 'internal', 'external' or 'both', with 'internal' by default)

See [Graphviz options](http://www.graphviz.org/doc/info/attrs.html) for more informations on **graph**, **node** and **edge** parameters.

# Examples #

Using `--all` and `--dependencies=both` options to center graph on depend\_graph module and show internal and external dependencies:

![https://lh5.googleusercontent.com/_zeWn5qPueCk/TaXVuBg__6I/AAAAAAAADis/2Vh1fnnCZTQ/graph_depend_graph_app.png](https://lh5.googleusercontent.com/_zeWn5qPueCk/TaXVuBg__6I/AAAAAAAADis/2Vh1fnnCZTQ/graph_depend_graph_app.png)

[Youki](http://sourceforge.net/apps/mediawiki/youki/index.php) project:

[![](http://lh3.ggpht.com/_zeWn5qPueCk/TH_GfcWat4I/AAAAAAAADeU/do7NuUbFSzc/youki2.png)](http://sourceforge.net/apps/mediawiki/youki/index.php)