# A* Simulation
A simulation of the A* algorithm using ncurses.

![showcase](https://user-images.githubusercontent.com/108880623/177858753-734d35dd-0a97-4666-8fe2-7e2ad217e3de.gif)

## Getting started
```
$ cd a-star-simulation

$ make

$ ./astar
```

```
Select a map (Default: 1, standard maps 1-5) [Input: 1]: 4

Delay between steps (Default: 300ms) [Input: 300]: 300

Cell size (Default: 3) [Input: 3]: 3
```

## Creating your own maps
You can also create your own maps. Mark the start node with 's', the end node with 'e' and walls with 'x'. Fill up each line with spaces. The last line is not followed by '\n'.

Example:
```
s         
  xxx     
          
xxx       
         e
```
