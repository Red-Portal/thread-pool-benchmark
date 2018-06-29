
# Thread Pool Benchmark
A C++ Thread Pool Colosseum, </br>
where various C++ thread pool implementations fight for glory and fortune.</br>

## Gladiators
* C++11 Threads </br>
* Sean Parent's implementation as suggested in [Boostcon](https://youtu.be/32f6JrQPV8c)
* [Grand Central Dispatch](https://github.com/apple/swift-corelibs-libdispatch), Apple
* ~~[HPX](https://github.com/STEllAR-GROUP/hpx), STEllAR-GROUP~~ To be added

## Example Results 
Intel Core i7-7700HQ, Manjaro Linux. </br>
1024 x 1024 Matrix Multiplication Event. </br>
 </br>
     
|         | forking | joining | total |
|---------|---------|---------|-------|
| C++ thread             | 14:754:27 | 2881:658:460 | 2896:412:487 |
| Sean Parent Naive      | 1:258:388 | 2946:454:719 | 2947:713:107 | 
| Sean Parent Multiqueue | 1:279:541 | 2934:516:668 | 2935:796:209 |
| Sean Parent Work Steal | 2:428:356 | 2911:868:401 | 2914:296:757 |
| Grand Central Dispatch | 0:986:258 | 2813:885:471 | 2814:871:729 |

## Fight Events
* Matrix Multiplication </br>

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
