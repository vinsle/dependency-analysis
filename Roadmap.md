# Current version (V1) #

## Packaging ##

  * ~~Add an ant build.xml for automatic compilation and packaging on build server~~
    * ~~Add [CppNcss](http://sourceforge.net/projects/cppncss/) metrics on post build~~
    * ~~Add self dependency-analysis metrics on post build~~
    * ~~Add [doxygen](http://www.stack.nl/~dimitri/doxygen/) documentation generation on post build~~
    * ~~Add an ant target for packaging (zip)~~
    * ~~Compile with static versions of Xerces~~
  * ~~Delete xerces dll versionning~~
  * ~~Rename depend\_app.exe into depend.exe~~
  * ~~Add the project on build server~~
    * Build on cygwin, ~~vc80~~ and linux platforms
    * Add binaries on download page
    * Add documentation on download page
  * ~~Add a version number (major version . svn revision) + a command line parameter to retrieve it~~
  * Add wiki documentation
    * How to use the tool
    * How to compile (vc80, ant)
    * Dependencies
    * Fill doxygen fields
  * Add licence files and readme.txt
  * ~~Clean graphviz warnings ("GLib-WARNING : Passing a non-NULL package to g\_win32\_get\_package\_installation\_directory() is deprecated and it is ignored.")~~

## Features ##

  * ~~Add png output format~~
    * ~~Use [Graphviz](http://www.graphviz.org/pdf/libguide.pdf) as a library~~
    * ~~Add some command line parameters to tweak the output (color, layout, size, labels, ...)~~
  * ~~Automate graph generation~~
    * ~~Add a command line parameter to generate a sub graph with a set of modules and their afferent and efferents modules~~
    * ~~Embed stylesheets to transform xml output into [Dot language](http://www.graphviz.org/doc/info/lang.html) and apply them with [XeuSeuLeu](http://sourceforge.net/apps/mediawiki/xeumeuleu/index.php?title=Patterns#XSL)~~
    * ~~Add a command line parameter to specify desired output format: xml or dot~~
  * ~~Add a verbose command line parameter~~
  * ~~User can specify multiple outputs (xml and png)~~
  * ~~User can specify an already computed xml (--input=already.xml) to avoid recomputation~~
    * ~~Generate any graph from this file (even with filtering)~~
  * ~~Use a configuration file (--configuration=...)~~
    * See [Program options storage](http://www.boost.org/doc/libs/1_44_0/doc/html/program_options/overview.html#id2084587)
    * ~~Add a --save-configuration=... to save current configuration file~~
    * ~~Read DEPEND environment variable~~
  * ~~Resolve external includes~~
    * ~~Internal and external includes are not detected with double quotes or angle brackets anymore~~
    * ~~Print a warning if an include cannot be resolved (file name, line number), must be case sensitive~~
    * ~~Add a command line parameter to specify include directories~~
  * ~~Add a command line parameter to make only cyclic dependencies detection between modules and print a warning (in this case no serialization)~~
  * ~~Add the same features and graph for internal class dependencies in a module~~
    * Detect cycles in includes at class level
  * ~~Add a command line parameter to disable warnings~~
  * Add a command line parameter to mutate warnings into execution errors

## Issues ##

  * Add a unique identifier system to avoid dependency conflict between internal and external modules (ie: with the same name, #include "tools/tools.h" vs #include <tools/other-tools.h>
  * Handle includes with multiple '/' or '\' (#include "test///module\\\include.h")
  * Handle multiple sub directories include in same module (internal include resolution must take the file path)

## Refactoring ##
  * ~~All options are given in xml format to Facade and forwarded to all modules~~