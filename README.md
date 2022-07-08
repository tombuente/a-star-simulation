# A* Simulation
A simulation of the A* algorithm using ncurses.

![astar](https://user-images.githubusercontent.com/108880623/177988125-f8ffb96e-1ab2-476d-be10-a40c19df8cc7.gif)

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
