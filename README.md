# Five-in-a-Row #

It is a simple Five-in-a-Row game written in MFC.

## How to build ##

This project use cmake build system. First of all, you should install
MSVC 11 or later with MFC and have cmake executable file. Change to the
directory where *CMakeLists.txt* exists, and type in command line:

``` console
cmake -S . -B build
```

for 32 bits or

``` console
cmake -S . -B build -A x64
```

for 64bits.

When the command above finishes successfully, type

``` console
cmake --build build --config Release --target install
```

You will get Five-in-a-Row.exe in the bin\Release directory. The pre-built binaries can be download here: [executable files](https://github.com/YiGeeker/Five-in-a-Row_MFC/releases)

## License ##

The GNU General Public License v3.

