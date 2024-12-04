<!--
MIT License

Copyright (c) 2024 Czech Technical University in Prague

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.-->
Integration with languages other than Java and C++
==================================================

In this project, we use `SWIG` to generate 'glue code' to integrate the `C++` code into `Java`. 
`SWIG` is a software development tool that simplifies integrating `C` and `C++` code with a variety of other languages. 
You can get more info about `SWIG` [here](http://www.swig.org/index.php) and the list of supported languages 
[here](http://www.swig.org/compat.html#SupportedLanguages).

`SWIG` uses `interface files` (with a suffix `.i`) to specify which classes and functions will be usable from the 
langauge we are integrating into. 
In our case, the interface is already prepared in the file `shortestPathsInterface.i` in this directory. 
This interface file can be used to to generate 'glue code' for any of the languages supported by `SWIG`.

To generate 'glue code' for example for `Python`, we could use: 

```
swig -python -c++ shortestPathsInterface.i`
```

Here the first argument denotes the programming language we are integrating into. 
The second argument is needed to distinguish between `C` and `C++` as the language of the code we are integrating. 
This call will generate new `shortestPathsInterface_wrap.cxx` file that will overwrite the existing one which is 
prepared for `Java`. 
The newly generated `shortestPathsInterface_wrap.cxx` will contain the wrappers for `Python`. 
The file additionally includes `Python.h`. 
Additionally, `shortestPaths.py` will be generated. 
This file needs to be included in the `Python` project in order to make the library usable from it.

After that, you will have to compile a new library, this means running the `shortestPaths` target from the 
`CMakeLists.txt` in the root directory of this project again. 
This run will use the new `shortestPathsInterface_wrap.cxx` so it will generate a library usable from the desired 
language. 
Depending on your chosen language, you might need to change the `CMmakeLists.txt` file to include new required packages.
For example for `Java`, `find_package(JNI REQUIRED)` is necessary. 
For other languages, other packages might be needed.

We have described the process on an example of integrating with `Python`. 
With other languages, the process is fairly similar. 
You start with `swig -[desired_language] -c++ shortestPathsInterface.i`. 
This will generate `shortestPathsInterface_wrap.cxx` containing wrappers for the desired language and some additional 
source code file in your chosen language (there can be more of them based on the language) that you have to include 
in your project.

Since the intention of this document is not to describe the whole process in depth, but only to point the interested 
reader in the right direction, we refer you to the [SWIG documentation](http://www.swig.org/doc.html) and the 
[SWIG tutorial](http://www.swig.org/tutorial.html) for futher information regarding this topic. 
