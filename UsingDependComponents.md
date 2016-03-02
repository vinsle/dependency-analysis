# Introduction #

**Depend components** is a tool made for automatic detection of [circular dependencies](http://en.wikipedia.org/wiki/Circular_dependency) (or [strongly connected components](http://en.wikipedia.org/wiki/Strongly_connected_component)) between modules. This tool will return an **error** code if any circular dependency is found and will produce an xml containing all components detected.

# Command line parameters #

`Usage: depend_components [options] --graph=graph.xml --output=components.xml`

  * **help**(-h): produce help message
  * **version**(-v): produce version message
  * **output**: set output components file
  * **graph**: set graph input file (from [depend](UsingDepend.md)) for analysis
  * **disable-warnings**: disable console warnings


# Example #

Here is an example with three modules with a circular dependency and one module out of this.

XML produced:
```
<strongly-connected-components>
    <component>
        <node>circular1</node>
        <node>circular2</node>
        <node>circular3</node>
     </component>
</strongly-connected-components>
```

Console output (warnings are activated by default):
```
Strongly connected component detected:
 - circular1
 - circular2
 - circular3
```

Dependency graph:

![http://lh4.googleusercontent.com/_zeWn5qPueCk/TaWkeO7e1tI/AAAAAAAADiM/I1_AgmudvQ4/circular_components.png](http://lh4.googleusercontent.com/_zeWn5qPueCk/TaWkeO7e1tI/AAAAAAAADiM/I1_AgmudvQ4/circular_components.png)