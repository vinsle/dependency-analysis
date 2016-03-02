# Modifying graph orientation #

First step: produce an xml from code using [depend application](UsingDepend.md):
`depend.exe depend/trunk/src/* --output=graph.xml`

By default, the generated graph has a top-down orientation:

`depend_graph.exe --output=depend.png graph.xml`

![http://lh4.ggpht.com/_zeWn5qPueCk/TH_GfKcMOVI/AAAAAAAADeM/uniju5AqweA/depend.png](http://lh4.ggpht.com/_zeWn5qPueCk/TH_GfKcMOVI/AAAAAAAADeM/uniju5AqweA/depend.png)

But with **rankdir** option `--graph="rankdir=LR"` (see [Graphviz documentation](http://www.graphviz.org/doc/info/attrs.html) for more informations), we can set the graph to a left-right orientation:

`depend_graph.exe --output=depend.png --graph="rankdir=LR" graph.xml`

![http://lh4.ggpht.com/_zeWn5qPueCk/TH_GfIswVeI/AAAAAAAADeQ/JyzkI9VgtUQ/depend2.png](http://lh4.ggpht.com/_zeWn5qPueCk/TH_GfIswVeI/AAAAAAAADeQ/JyzkI9VgtUQ/depend2.png)

# Add Qt external dependency #

Another exemple on a project using Qt (with special include directories and Qt moc objects generated and included in code):

`depend.exe --output=depend.xml --path=src/libraries --path=src/applications --path=out/release/applications --path=out/release/libraries --include=include --include="include/qt,Qt" --exclude="$VSINSTALLDIR}/VC/include" --exclude="$VSINSTALLDIR}/VC/PlatformSDK/Include" --warning`