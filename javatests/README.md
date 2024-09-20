A test Java application for the Shortest Path library
=====================================================

This directory contains a simple stand-alone `Java` application that tests that the library is working correctly. 
This application can also serve as an example for the integration with `Java`.

Building the application and running the tests
----------------------------------------------

To make the application running, you will need the `library` compiled for your architecture. 
This means a file `libshortestPaths.so` in Linux or `shortestPaths.dll` in Windows.
To run the `Java` application, first create a `JAVA_HOME` system property with the absolute path to the JDK, e.g., `C:\Program Files\Java\jdk-15.0.1`.
Compilation of this library is described in the [main readme](../README.md) of this project ([here](../README.md)).

The `Java` application needs to be able to find the library. 
You have to ensure, that the directory that contains the library will be included in the `java.library.path` 
system property when the Java Virtual Machine tries to load the library. 
One way to accomplish this is to set your enviroment variables in a way where the `java.library.path` will contain 
the path. 
You can also use the argument `-Djava.library.path="your_path_to_library"` as an VM option (this can be useful 
when running the application from an IDE). 
The process of the setting the enviroment variables correctly is described in more detail in the document about running 
the library with Amodsim [here](../AMOD_README.md).

After you are sure that the `java.library.path` will contain the path to the library, you can just call:

```
mvn test
```

This should invoke all the tests (currently there are 6 of them). 
All the tests should finish without failure. 
If the tests finish successfully, then your compiled library is working correctly.
