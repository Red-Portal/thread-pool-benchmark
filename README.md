
# Thread Pool Benchmark
A C++ Thread Pool Colosseum, </br>
where various C++ thread pool implementations fight for glory and fortune.</br>

## Gladiators
* C++11 Threads </br>
* Sean Parent's implementation as suggested in [Boostcon](https://youtu.be/32f6JrQPV8c)
* [Grand Central Dispatch](https://github.com/apple/swift-corelibs-libdispatch), Apple
* [Thread Building Blocks](https://github.com/01org/tbb), Intel 
* ~~[HPX](https://github.com/STEllAR-GROUP/hpx), STEllAR-GROUP~~ To be added

## Fight Events
* Matrix Multiplication </br>

## Example Results 
Intel Core i7-7700HQ, Manjaro Linux, clang 6.0.0 </br>
1024 x 1024 Matrix Multiplication Event. </br>
 </br>
     
|         | forking | joining | total |
|---------|---------|---------|-------|
| C++ thread             | 13:952:610 | 3234:782:593 | 3248:735:203 |
| Sean Parent Naive      |  1:163:562 | 3199:446:433 | 3200:609:995 | 
| Sean Parent Multiqueue |  1:689:353 | 3191:830:204 | 3193:519:557 |
| Sean Parent Work Steal |  1:172:337 | 3129:783:927 | 3130:956:264 |
| Grand Central Dispatch |  1:112:406 | 3034:694:607 | 3035:817: 13 |
| Thread Building Blocks |  2: 12:426 | 3165:723:263 | 3167:735:689 |

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
