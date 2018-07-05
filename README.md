
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
* 1024 * 1024 Matrix Multiplication </br>
* 2048 * 2048 Fluid Solver </br>

## Example Results 
Intel Core i7-7700HQ, Manjaro Linux, clang 6.0.0 </br>
 </br>
     
|   matrix multiplication   |   forking    |    joining   |     total    |
|---------------------------|--------------|--------------|--------------|
| C++ thread                | 473.692266ms |  14.826739ms | 488.519005ms |
| Sean Parent Naive         |   0.312164ms | 423.402574ms | 423.714738ms |
| Sean Parent Multiqueue    |   0.314355ms | 472.653375ms | 472.967730ms |
| Sean Parent Work Stealing |   0.292475ms | 443.798525ms | 444.091000ms |
| progschj's ThreadPool     |   0.286827ms | 562.374141ms | 562.660968ms |
| Grand Central Dispatch    |   0.374894ms | 423.493407ms | 423.868301ms |
| Thread Building Blocks    |   1.042765ms | 488.946873ms | 489.989638ms |

 </br>

|        fluid solver       |    forking    |    joining    |     total     |
|---------------------------|---------------|---------------|---------------|
| C++ thread                | 1252.285177ms |  375.896608ms | 1628.181785ms |
| Sean Parent Naive         |   19.664301ms | 1149.353465ms | 1169.017766ms |
| Sean Parent Multiqueue    |    6.706725ms | 1162.610971ms | 1169.317696ms |
| Sean Parent Work Stealing |    9.154444ms | 1149.240601ms | 1158.395045ms |
| progschj's ThreadPool     |    7.787490ms | 1559.932310ms | 1567.719800ms | 
| Grand Central Dispatch    |    9.418396ms | 1161.239495ms | 1170.657891ms |
| Thread Building Blocks    |   10.892387ms | 1224.662278ms | 1235.554665ms |

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
