# What is dependency-analysis? #
**Dependency-analysis** is a tool suite helping developpers to analyse dependencies and maintain quality on their project, including:
  * [Dependency detection](UsingDepend.md)
  * [Dependency guard](UsingDependGuard.md)
  * [Cyclic dependencies detection](UsingDependComponents.md)
  * [Abstractness and instability metrics computation](UsingDependMetrics.md)
  * [Dependency graph drawing](UsingDependGraph.md).

# Download #

Download it directly and feel free to read QuickStart:
  * [Depend for Windows](http://dependency-analysis.googlecode.com/files/documentation_v1.0.zip)
  * [Depend for Linux](http://dependency-analysis.googlecode.com/files/depend_linux_v1.0.zip)

# Example #
Using includes and [metrics](http://www.objectmentor.com/resources/articles/oodmetrc.pdf).
<font color='green'>Green</font> modules have a nice distance (0) and <font color='red'>red</font> modules have the worst distance (100).

Depend also detects cyclic dependencies between modules (sadely there is no cycle in the example below).

Example with [youki project](http://sourceforge.net/apps/mediawiki/youki/index.php):

[![](http://lh3.ggpht.com/_zeWn5qPueCk/TH_GfcWat4I/AAAAAAAADeU/do7NuUbFSzc/youki2.png)](http://sourceforge.net/apps/mediawiki/youki/index.php)