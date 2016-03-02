# Introduction #

**Depend metrics** is a tool made for dependency metrics computation.

8 metrics are computed for each module:
  * Number of afferent dependencies
  * Number of efferent dependencies
  * Number of external dependencies
  * Number of classes
  * Number of abstract classes
  * Abstractness
  * Instability
  * Distance

Example of metrics output:
```
<metrics>
  <metric name="depend">
    <afferent>49</afferent>
    <efferent>0</efferent>
    <external>90</external>
    <classes>53</classes>
    <abstract-classes>18</abstract-classes>
    <abstractness>33</abstractness>
    <instability>0</instability>
    <distance>67</distance>
  </metric>
  ...
</metrics>
```

See UnderstandingMetrics for more informations on metrics.

# Command line parameters #
`Usage: depend_metrics [options] --output=metrics.xml --graph=graph.xml`

  * **help**(-h): produce help message
  * **version**(-v): produce version message
  * **graph**: set graph input file for analysis
  * **output**: set output metrics file ( the result is printed on standard output by default ).
  * **filter**: select only modules in filter and their afferent and efferent modules
  * **extend**: extend to all reachable modules if filter is enabled

See UnderstandingFilters for more informations on **filter** and **extend** options.