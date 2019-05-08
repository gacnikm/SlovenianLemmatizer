LemmaGen Slovene Lemmatizer module
===========================================
[![Build Status](https://travis-ci.org/izacus/SlovenianLemmatizer.svg?branch=master)](https://travis-ci.org/izacus/SlovenianLemmatizer)

This is a lemmatizer for slovene language.

The file directory structure is as follows:

```
java/       - Java JNI bindings and Solr 4.x module
python/     - Python bindings

data/         - Binary language dictionary files required by the lemmatizer
..about.txt   - Data file key

src/          - Lemmatizer C++ source

CMakeLists.txt  - CMake file to build lemmatizer libraries and binaries,SOLR JNI libraries and binaries
pom.xml       - Maven POM to build Solr module, SOLR and JNI binding JAR
setup.py       - build Python package
```

Compilation
----------------

Calling `nmake`(Win) / `make`(Lin) will build
 * `rdrlemmatizer.lib` - static RdrLemmatizer library for 3rd party usage
 * `lemmagen.so/dll` - shared library for 3rd party usage
 * `lemmatizer.so/dll`         library to use with SOLR
 * `lemmagen_bin`         executable to lemmatize words from commandline
 * `lemmagen_tests`         executable to to perform tests
Tested on MSVC++ 19.16.27030.1, v15.00.30729.01, and WSL Ubuntu 18.04.


Solr/Java JAR is built by calling `mvn compile` in directory with `pom.xml`.
Python package is built by calling `python setup.py install`.

SOLR Usage
----------------
- copy lemmatizer.so/dll, sl_lemmatizer-1.0.jar to lib subfolder in your SOLR core
- Add lemmatization filter to schema.xml:
```<filter class="si.virag.solr.RdrLemmatizerFactory" dictionary="/absolute/path/to/dictionary" />```
- start solr with argument ```-a "-Djava.library.path=/path/to/core/lib/folder"```

Contributors:
----------------

Original version form Joseph Stephan Institute (http://lemmatise.ijs.si/)

* Jernej Virag
* Domen Grabec
* Gašper Žejn
