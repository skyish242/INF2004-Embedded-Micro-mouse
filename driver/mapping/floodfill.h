#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include <stdio.h>
#include <stdbool.h>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN -1
#define FLOOD_ONE_CELL 9050 // Envoder value

// Define size of maze
#define WIDTH 8
#define HEIGHT 8 

// Define the start coord 
#define START_X 0
#define START_Y 0

// Define the end coord
#define END_X 8
#define END_Y 8

// Define visits
#define VISITED 1
#define NOT_VISITED 0

// Define cell length
#define MAX_CELL_LENGTH 17

// define info for each cell
struct cell_info{
	// Indication for north,east,south,west walls
	bool walls[4];
	bool visited;
};

// struct to hold cell info
struct wall_maze{
	struct cell_info cells[WIDTH][HEIGHT]; //No of cells in maze
};

// struct to hold distance info
// HEIGHT is the number of rows
// WIDTH is the number of columns
struct dist_maze{
	int distance[HEIGHT][WIDTH];
};

// struct to indicate coordinates of cell
struct coor{
	int x;
	int y;
};

#endif /* FLOODFILL_H_ */