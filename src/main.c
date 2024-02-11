#include <stdio.h>
#include <stdlib.h>

#include "bfs.h"

int main(int argc, char* argv[]) {

	int n, m;
	Cell** cells;
	FILE* input_file;

	if (argc != 2) {
		fprintf(stderr, "Missing input file name. Usage: main.exe <input file>");
		return EXIT_FAILURE;
	}

	input_file = fopen(argv[1], "r");
	if (input_file == NULL) {
		fprintf(stderr, "Unable to open the file %s", argv[1]);
		return EXIT_FAILURE;
	}

	fscanf(input_file, "%d %d ", &n, &m);

	cells = create_matrix(n, m);
	init_matrix(cells, n, m, input_file);
	fclose(input_file);

	build_adjacency_lists(cells, n, m);
	bfs(cells);
	print_shortest_path(cells, n, m);

	deallocate_matrix(cells, n, m);
	free(cells);

	return EXIT_SUCCESS;
}
