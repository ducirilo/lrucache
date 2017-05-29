# lrucache
A simple C++ implementation for a Least Recently Used (LRU) Cache.


## Compiling and running
### If you are on a Linux distribution:

1. Open a terminal at the directory where the project's sources have been downloaded
2. In the project's root directory , execute the following commands (make sure you have `g++` compiler installed in your environment):

```bash
  g++ -o main main.cpp
  ./main
```
The source code file `main.cpp` will be compiled producing the executable `main`, which runs immediately afterwards.

### If you are on a Windows system:

1. Open a command prompt window at the directory where the project's sources have been downloaded
2. Make sure you meet all prerequisites for compiling native C++ program in your environment with `cl.exe` compiler (please, see [this link for further details](https://msdn.microsoft.com/en-us/library/ms235639.aspx)).
3. If everything is fine, in the project's root directory , execute the following commands:

```bash
  cl /EHsc main.cpp
```
The `cl.exe` compiler generates an .obj file that contains the compiled code, and then runs the linker to create an executable proram named `main.exe`. To run this program at the command prompt, just type the following line in the shell:

```bash
  main
```
The program will be executed and its output will be shown on the prompt window.


## Playing with the code
The executable program will run all instructions declared in the function `main()` of the source code file `main.cpp`, including:

```cpp
const unsigned N =4;

LRUCache lruCache;

lruCache.build(N);

lruCache.add(0, "Marvin");
lruCache.add(1, "Ford Prefect");
lruCache.add(0, "Another Marvin");
lruCache.add(10, "Lisbeth Salander");
lruCache.add(3, "Mikael Blomkvist");
lruCache.add(4, "Trician McMillian");
lruCache.add(5, "Don't panic!");

try {
  lruCache.get(3);
  lruCache.get(11);
 } catch (const std::invalid_argument& e) {
  cerr << "An error has ocurred while reading data from cache: " << e.what() <<  endl;
}

lruCache.destruct();
```
In order to test different behaviors, just update the function `main()`'s content, by changing the cache's capacity (expressed in the constant `N`) and rebuilding it with operation `build(unsigned n)`; or even by inserting and reading data with distinct keys using the operations `add(unsigned key, string data)` and `get(unsigned key)`. You may also use the operation `destruct()` to clear all cache data.

Once you have changed the function `main()` with new contents, do not forget to recompile your code before running it to see the new results.

