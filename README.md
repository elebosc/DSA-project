# DSA Project

This project has been realized for the Data Structures and Algorithms course (A.Y. 2021/22) of the Bachelor's degree in Computer Science and Engineering at University of Bologna, Cesena Campus.

## Scenario

The subject of the considered scenario is a robotic vacuum cleaner.

The environment in which the robot moves is represented by a ${n \times m}$ matrix, inside of which the free spaces and the obstacles are properly distinguished.

The robot is considered to occupy a space of ${3 \times 3}$ cells, it is able to move cell by cell in four directions (North, East, South and West) and it is supposed to start from the top-left corner of the environment. In the bottom-right corner of the environment, there is the charging point of the robot.

The aim of this project is to realize an algorithm to compute the shortest path that the robot has to follow to reach the recharging point avoiding all obstacles, if possible; if the positioning of the obstacles makes it impossible, the algorithm must report that.

The provided solution makes use of the BFS (Breadth-First Search) Algorithm.

## How to build

1. Install ``make`` tool.

2. On the command line, run ``make`` in the project root directory.

## How to run

Usage: ``main.exe <input file>``

Some valid input files (``.in`` files) are provided in the ``test_files`` folder, along with the ``.out`` files containing the expected output.

## Output

If a path is found, the output has the following format:
```
<n-steps>
<path>
```
where ``n-steps`` is the number of steps of the found shortest path and ``path`` is a string that represents the steps of the path (where N is North, E is East, S is South and W is West) .

If no path can be found, ``-1`` is printed.
