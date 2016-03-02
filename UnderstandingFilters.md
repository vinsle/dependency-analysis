# Introduction #

**Filter** and **Extend** options can be used to focus analysis on one (or more) module and its dependencies.

The applications using **Filter** and **Extend** options are:
  * depend\_metrics
  * depend\_graph

## Filter option ##

Calling filter option with a module name will define this module as a _root_ module, which is automatically added. Direct afferent and efferent dependencies of this (these) root(s) module(s) are also selected.

`application --filter=module_name`.

Many filters can be added:

`application --filter=module1 --filter=module2`.

## Extend option ##

# Example #

## All the modules ##

```
<nodes>
  <node name="filtered">
    <efferent-dependencies/>
    <external-dependencies/>
  </node>
  <node name="afferent-transitive-dependency">
    <efferent-dependencies>
      <dependency name="afferent-dependency">
        <context>context</context>
      </dependency>
    </efferent-dependencies>
    <external-dependencies/>
  </node>
  <node name="afferent-dependency">
    <efferent-dependencies>
      <dependency name="center">
        <context>context</context>
      </dependency>
    </efferent-dependencies>
    <external-dependencies/>
  </node>
  <node name="center">
    <efferent-dependencies>
      <dependency name="efferent-dependency">
        <context>context</context>
      </dependency>
    </efferent-dependencies>
    <external-dependencies/>
  </node>
  <node name="efferent-dependency">
    <efferent-dependencies>
      <dependency name="efferent-transitive-dependency">
        <context>context</context>
      </dependency>
    </efferent-dependencies>
    <external-dependencies/>
  </node>
  <node name="efferent-transitive-dependency">
    <efferent-dependencies/>
    <external-dependencies/>
  </node>
</nodes>
```

![http://lh4.googleusercontent.com/_zeWn5qPueCk/TaXdRadtNVI/AAAAAAAADjA/3PYz39TkSiU/entire.png](http://lh4.googleusercontent.com/_zeWn5qPueCk/TaXdRadtNVI/AAAAAAAADjA/3PYz39TkSiU/entire.png)

This graph was rendered using the following command line:

`depend_graph --output=all_nodes.png dependencies.xml`

## Filter ##

![http://lh6.googleusercontent.com/_zeWn5qPueCk/TaXdRJ4Eg-I/AAAAAAAADi8/sj3V6lRSRVM/filter.png](http://lh6.googleusercontent.com/_zeWn5qPueCk/TaXdRJ4Eg-I/AAAAAAAADi8/sj3V6lRSRVM/filter.png)

This graph was rendered using the following command line:

`depend_graph --output=all_nodes.png dependencies.xml --filter=center`

![https://lh4.googleusercontent.com/_zeWn5qPueCk/Taa_6Y8EeLI/AAAAAAAADjg/mQGW6p0-A0A/filtered.png](https://lh4.googleusercontent.com/_zeWn5qPueCk/Taa_6Y8EeLI/AAAAAAAADjg/mQGW6p0-A0A/filtered.png)

And this one was rendered using the following command line:

`depend_graph --output=all_nodes.png dependencies.xml --filter=filtered`

## Extended ##

![https://lh6.googleusercontent.com/_zeWn5qPueCk/TaXj4IOQcbI/AAAAAAAADjQ/iNEPBILxHLc/extend.png](https://lh6.googleusercontent.com/_zeWn5qPueCk/TaXj4IOQcbI/AAAAAAAADjQ/iNEPBILxHLc/extend.png)

This graph was rendered using the following command line:

`depend_graph --output=all_nodes.png dependencies.xml --filter=center`