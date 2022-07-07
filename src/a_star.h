#ifndef A_STAR_H
#define A_STAR_H

#include <stdbool.h>

typedef struct Node {
	int x;
	int y;

	int g;	// Distance from starting node
	int h;	// Distance from end node
	int f;	// G-cost + F-cost

	bool walkable;

	struct Node* parent;
} Node;

typedef struct {
	int size_x;
	int size_y;

	Node* start;
	Node* end;

	Node* nodes;

	int cell_size;

	struct timespec* ts;
} Environment;

/**
 * @brief Initialize an Environment.
 *
 * @param size_x Size in X direction
 * @param size_y Size in Y direction
 * @return Environment*
 */
Environment* init_environment(int size_x, int size_y);

/**
 * @brief Set the start and end node of an Environment.
 *
 * @param env Environment
 * @param start_x X coordinate of start node
 * @param start_y Y coordinate of start node
 * @param end_x X coordinate of end node
 * @param end_y Y coordinate of end node
 */
void set_start_and_end(Environment* env, int start_x, int start_y, int end_x, int end_y);

/**
 * Starts an A* simulation.
 */
void run();

#endif	// A_STAR_H