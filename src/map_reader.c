#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_star.h"

const char start = 's';
const char end = 'e';
const char wall = 'x';

Environment* init_env_from_map(char* filename) {
	FILE* map;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	map = fopen(filename, "r");
	if (map == NULL) {
		exit(EXIT_FAILURE);
	}

	int size_x = 0;
	int size_y = 0;

	size_x = getline(&line, &len, map) - 1;	 // Remove '\n'
	rewind(map);

	while (getline(&line, &len, map) != -1) {
		size_y++;
	}

	Environment* env = init_environment(size_x, size_y);

	int start_x = 0;
	int start_y = 0;
	int end_x = 0;
	int end_y = 0;

	int y = 0;
	Node* curr;
	int x;
	rewind(map);
	while ((read = getline(&line, &len, map)) != -1) {
		line[strcspn(line, "\n")] = 0;
		for (x = 0; x < env->size_x; x++) {
			if (line[x] == start) {
				start_x = x;
				start_y = y;
			} else if (line[x] == end) {
				end_x = x;
				end_y = y;
			} else if (line[x] == wall) {
				curr = &env->nodes[y * env->size_x + x];
				curr->walkable = false;
			}
		}
		y++;
	}

	set_start_and_end(env, start_x, start_y, end_x, end_y);

	fclose(map);

	if (line) {
		free(line);
	}

	return env;
}