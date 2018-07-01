
# Thread Pool Benchmark
A C++ Thread Pool Colosseum, </br>
where various C++ thread pool implementations fight for glory and fortune.</br>

## Install

```shell
git clone --recursive https://github.com/Red-Portal/thread-pool-benchmark.git
cmake -G "<Generator>"
make -j4
```

## Gladiators
* C++11 Threads </br>
* Sean Parent's implementation as suggested in [Boostcon](https://youtu.be/32f6JrQPV8c)
* [Grand Central Dispatch](https://github.com/apple/swift-corelibs-libdispatch), Apple
* [Thread Building Blocks](https://github.com/01org/tbb), Intel 
* [ThreadPool](https://github.com/progschj/ThreadPool), progschj
* ~~[HPX](https://github.com/STEllAR-GROUP/hpx), STEllAR-GROUP~~ To be added


## Fight Events
* Matrix Multiplication </br>

## Example Results 
Intel Core i7-7700HQ, Manjaro Linux, clang 6.0.0 </br>
1024 x 1024 Matrix Multiplication Event. </br>
respectively ms \: us \: ns  </br>
 </br>
     
|                           |   forking    |    joining   |     total    |
|---------------------------|--------------|--------------|--------------|
| C++ thread                | 350.864608ms | 117.121797ms | 467.986405ms |
| Sean Parent Naive         |   0.346831ms | 386.044511ms | 386.391342ms |
| Sean Parent Multiqueue    |   0.361352ms | 451.210573ms | 451.571925ms |
| Sean Parent Work Stealing |   0.357595ms | 423.890832ms | 424.248427ms |
| progschj's ThreadPool     |   0.364707ms | 536.531084ms | 536.895791ms |
| Grand Central Dispatch    |   0.519471ms | 393.838209ms | 394.357680ms |
| Thread Building Blocks    |   2.811747ms | 466.988086ms | 469.799833ms |

## Contributions
All contributions are welcome, </br>
including new types of benchmark problems and publicly available thread pool implementations</br>

## LICENSE

 ```
 thread-pool-benchmark, a C++ Thread Pool Colosseum
 Copyright (C) 2018  Red-Portal
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ```
