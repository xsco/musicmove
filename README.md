musicmove
=========

musicmove is a tool that can rename and move music files into a directory
structure that aligns with their metadata tags.  If you consider the metadata
in your music files to the definitive information about your music, then
musicmove can help get the filesystem to match.

musicmove is free software, meaning that anyone can use, share, and modify it.
It is licensed under the GNU General Public License (GPL), version 3.

For more information, see the main website at http://musicmove.xsco.org

Compilation
-----------

musicmove is built using CMake.  Normally, the following commands, issued in
the base project directory, should be sufficient to configure and build the
package:

```
    $ mkdir cmake-build && cd cmake-build
    $ cmake ..
    $ cmake --build .
```

You can then install the package (with sufficient privileges) by issuing:

```
    # cmake --install .
```

Dependencies
------------

musicmove requires suitable versions of the following libraries:

* [Boost](http://www.boost.org)
* [TagLib](http://taglib.org)
* [ChaiScript](http://chaiscript.com)

The CMake configuration step should report an error if any of the necessary
dependencies are missing.

Usage
-----

Refer to the [online manual](http://musicmove.xsco.org/docs) for information
on how to use musicmove.
