# Conflict-based-search

吾人自己实现的cbs算法

[![Build](https://github.com/whoenig/libMultiRobotPlanning/actions/workflows/build.yml/badge.svg)](https://github.com/whoenig/libMultiRobotPlanning/actions/workflows/build.yml)

The following algorithms are currently supported:

* Single-Robot Algorithms
  * A*
* Multi-Robot Algorithms
  * Conflict-Based Search (CBS)

## Building

Build on Ubuntu 22.04.

```shell
cmake .
make
```

## Run example instances

### A\*

```shell
./a_star_mine
```

### CBS

````shell
./cbs_mine -i ./benchmark/8x8_obst12_txt/map_8by8_obst12_agents8_ex10.txt -o output.yaml
````

