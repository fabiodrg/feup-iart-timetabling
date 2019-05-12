# feup-iart-timetabling

Problem website: http://sferics.idsia.ch/Files/ttcomp2002/oldindex.html

## Compiling

In order to compile simply run `$ make` on the project top directory. A new soft-link, `main`, is generated.

By default, the compilation used is `release`, i.e., no debug information is generated. If that information is needed, use the `debug` compilation level, by running `$ make debug`.

The compiler used is `g++` from GNU. The C++ version is 11.

## Generating documentation

Makefile has instructions to generate documentation using **Doxygen**. Simply run `$ make docs`. All documentation is generated under `docs` folder.

Ensure **Doxygen** is installed and that the executable `doxygen` is present at `PATH`.

## Fetching problem input examples

The Makefile also has support for downloading the input example files from the original competition website. Execute `$ make examples`.

The `examples` task assume `wget` and `unzip` are available on the system. The files are extracted to a folder `examples`.
