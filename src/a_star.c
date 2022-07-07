#include "a_star.h"

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "linked_list.h"
#include "map_reader.h"

const int max_neighbors = 8;
int f_size = 9999999;  // Maximum f_cost

// clang-format off
const char* title_string =
"\
    ###         ######  ########    ###    ########\n\
   ## ##       ##    ##    ##      ## ##   ##     ##\n\
  ##   ##      ##          ##     ##   ##  ##     ##\n\
 ##     ##      ######     ##    ##     ## ########\n\
 #########           ##    ##    ######### ##   ##\n\
 ##     ##     ##    ##    ##    ##     ## ##    ##\n\
 ##     ##      ######     ##    ##     ## ##     ##";
// clang-format on

void destroy_environment(Environment* env);

void a_star(Environment* env);

int distance_between_nodes(Node* a, Node* b);
void update_costs(Node* node, int g, int h);

void get_neighbors(Environment* env, Node** neighbors, Node* node);
void clear_neighbors(Node** neighbors);

// TUI
void print_grid(Environment* env);
void print_path(Environment* env, Node* node);
void clear_path(Environment* env, Node* node);

void run() {
	initscr();
	cbreak();
	curs_set(1);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_CYAN);	// start and end
	init_pair(2, COLOR_WHITE, COLOR_BLACK);	 // not visited
	init_pair(3, COLOR_WHITE, COLOR_GREEN);	 // visted
	init_pair(4, COLOR_WHITE, COLOR_MAGENTA);  // path

	mvprintw(1, 0, title_string);

	int buf_size = 256;
	char input_buf[buf_size];

	memset(input_buf, '\0', sizeof(char) * buf_size);

	// Select map
	mvprintw(12, 0, "Select a map (Default: 1, standard maps 1-5) [Input: 1]: ");
	getstr(input_buf);
	if (input_buf[0] == '\0') {
		input_buf[0] = '1';
	}
	char map_path[buf_size];
	snprintf(map_path, sizeof(map_path), "maps/%s.txt", input_buf);
	Environment* env = init_env_from_map(map_path);

	memset(input_buf, '\0', sizeof(char) * buf_size);

	// Select delay
	mvprintw(14, 0, "Delay between steps (Default: 300ms) [Input: 300]: ");
	getstr(input_buf);
	if (input_buf[0] == '\0') {
		input_buf[0] = '3';
		input_buf[1] = '0';
		input_buf[2] = '0';
	}
	int millis = strtol(input_buf, NULL, 10);
	env->ts->tv_sec = millis / 1000;
	env->ts->tv_nsec = (millis % 1000) * 1000000;

	memset(input_buf, '\0', sizeof(char) * buf_size);

	// Select cell size
	mvprintw(16, 0, "Cell size (Default: 3) [Input: 3]: ");
	getstr(input_buf);
	if (input_buf[0] == '\0') {
		input_buf[0] = '3';
	}
	env->cell_size = (int)strtol(input_buf, NULL, 10);

	// Clear stdwin and hide cursor
	erase();
	curs_set(0);

	a_star(env);

	getch();
	destroy_environment(env);
	endwin();
}

Environment* init_environment(int size_x, int size_y) {
	Environment* env = malloc(sizeof(*env));
	env->nodes = malloc(size_x * size_y * sizeof(Node));
	env->ts = malloc(sizeof(struct timespec));

	env->size_x = size_x;
	env->size_y = size_y;

	Node* node;
	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			node = &env->nodes[y * size_x + x];

			node->x = x;
			node->y = y;
			node->f = f_size;
			node->walkable = true;
		}
	}

	return env;
}

void destroy_environment(Environment* env) {
	free(env->nodes);
	free(env->ts);
	free(env);

	return;
}

void a_star(Environment* env) {
	print_grid(env);

	ListElement* open = init_linked_list();	 // only insert_sorted
	ListElement* closed = init_linked_list();  // only insert

	Node* neighbors[8];
	clear_neighbors(neighbors);

	int g;
	int h;

	Node* cur;

	open = insert_sorted(open, env->start);
	while (true) {
		if (is_empty(open)) {
			mvprintw(env->size_y + 3, 0, "No path!!!");
			return;
		}

		cur = open->data;
		open = remove_first(open);
		closed = insert(closed, cur);

		if (cur == env->end) {
			break;
		}

		clear_neighbors(neighbors);
		get_neighbors(env, neighbors, cur);
		for (int i = 0; i < max_neighbors; i++) {
			if (!neighbors[i]) {
				continue;
			}

			// If neighbor is not walkable or is in closed
			if (!neighbors[i]->walkable || contains(closed, neighbors[i])) {
				continue;
			}

			// distanceance from start node
			g = cur->g + distance_between_nodes(cur, neighbors[i]);
			// distanceance from end node
			h = distance_between_nodes(env->end, neighbors[i]);

			if (neighbors[i]->f > g + h || !contains(open, neighbors[i])) {
				update_costs(neighbors[i], g, h);
				neighbors[i]->parent = cur;
			}

			// TUI
			if (neighbors[i] != env->end) {
				attron(COLOR_PAIR(3));
				mvprintw(neighbors[i]->y, neighbors[i]->x * (env->cell_size + 1), "%0*d", env->cell_size, neighbors[i]->f);
				attroff(COLOR_PAIR(3));
				refresh();
			}
			// END TUI

			if (!contains(open, neighbors[i])) {
				open = insert_sorted(open, neighbors[i]);
			}
		}

		// TUI
		print_path(env, cur);
		refresh();
		clear_path(env, cur);
		// END TUI

		nanosleep(env->ts, NULL);
	}

	print_path(env, env->end->parent);
	mvprintw(env->size_y + 3, 0, "Found path!");
	refresh();

	destory_linked_list(open);
	destory_linked_list(closed);
	return;
}

void set_start_and_end(Environment* env, int start_x, int start_y, int end_x, int end_y) {
	env->start = &env->nodes[start_y * env->size_x + start_x];
	env->end = &env->nodes[end_y * env->size_x + end_x];

	update_costs(env->start, 0, 0);	 // Important in a_star() cur->g, because cur = env->start in first iteration
}

int distance_between_nodes(Node* a, Node* b) {
	int distX = abs(a->x - b->x);
	int distY = abs(a->y - b->y);

	if (distX > distY) {
		return 14 * distY + 10 * (distX - distY);
	}

	return 14 * distX + 10 * (distY - distX);
}

void update_costs(Node* node, int g, int h) {
	node->g = g;
	node->h = h;
	node->f = g + h;
}

void get_neighbors(Environment* env, Node** neighbors, Node* node) {
	// clang-format off
    //
    // 1   2   3
    // 4       5
    // 6   7   8
    //
    // 0;0 1;0 2;0    nodes + off - size_x - 1    nodes + off - size_x    nodes + off - size_x + 1
    // 0;1     2;1    nodes + off - 1                                     nodes + off + 1
    // 0;2 1;2 2;2    nodes + off + size_x - 1    nodes + off + size_x    nodes + off + size_x + 1
    //
	// Where nodes = env->nodes
    //
	// clang-format on

	// Current offset in grind relative to env->nodes
	int off = node->y * env->size_x + node->x;

	int calc_off;  // Calculated offset

	int size = env->size_x * env->size_y;

	// 1
	calc_off = off - env->size_x - 1;
	if (calc_off >= 0 && calc_off >= (node->y - 1) * env->size_x) {
		neighbors[0] = env->nodes + calc_off;
	}

	// 2
	calc_off = off - env->size_x;
	if (calc_off >= 0) {
		neighbors[1] = env->nodes + calc_off;
	}

	// 3
	calc_off = off - env->size_x + 1;
	if (calc_off >= 0 && calc_off < (node->y * env->size_x)) {
		neighbors[2] = env->nodes + calc_off;
	}

	// 4
	calc_off = off - 1;
	if (calc_off >= 0 && calc_off >= node->y * env->size_x) {
		neighbors[3] = env->nodes + calc_off;
	}

	// 5
	calc_off = off + 1;
	if ((calc_off < size) && (calc_off < (node->y + 1) * env->size_x)) {
		neighbors[4] = env->nodes + calc_off;
	}

	// 6
	calc_off = off + env->size_x - 1;
	if (calc_off < size && calc_off >= (node->y + 1) * env->size_x) {
		neighbors[5] = env->nodes + calc_off;
	}

	// 7
	calc_off = off + env->size_x;
	if (calc_off < size) {
		neighbors[6] = env->nodes + calc_off;
	}

	// 8
	calc_off = off + env->size_x + 1;
	if (calc_off < size && calc_off < (node->y + 2) * env->size_x) {
		neighbors[7] = env->nodes + calc_off;
	}

	return;
}

void clear_neighbors(Node** neighbors) {
	memset(neighbors, 0, 8 * sizeof(Node*));
}

void print_grid(Environment* env) {
	int size_x = env->size_x;
	int size_y = env->size_y;

	char* placeholder = "                ";

	Node* node;
	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			node = &(env->nodes)[y * size_x + x];
			if (node->walkable) {
				mvprintw(y, x * (env->cell_size + 1), "%0*d", env->cell_size,
					node->f % (int)pow(10, env->cell_size));
			}
			if (node == env->start) {
				attron(COLOR_PAIR(1));
				mvprintw(y, x * (env->cell_size + 1), "%.*s", env->cell_size,
					placeholder);
				mvprintw(y, x * (env->cell_size + 1) + env->cell_size / 2, "s");
				attroff(COLOR_PAIR(1));
			}
			if (node == env->end) {
				attron(COLOR_PAIR(1));
				mvprintw(y, x * (env->cell_size + 1), "%.*s", env->cell_size,
					placeholder);
				mvprintw(y, x * (env->cell_size + 1) + env->cell_size / 2, "e");
				attroff(COLOR_PAIR(1));
			}
		}
	}

	return;
}

void print_path(Environment* env, Node* node) {
	attron(COLOR_PAIR(4));
	while (node != env->start) {
		mvprintw(node->y, node->x * (env->cell_size + 1), "%0*d",
			env->cell_size, node->f);
		node = node->parent;
	}
	attroff(COLOR_PAIR(4));

	return;
}

void clear_path(Environment* env, Node* node) {
	attron(COLOR_PAIR(3));
	while (node != env->start) {
		mvprintw(node->y, node->x * (env->cell_size + 1), "%0*d",
			env->cell_size, node->f);
		node = node->parent;
	}
	attroff(COLOR_PAIR(3));

	return;
}