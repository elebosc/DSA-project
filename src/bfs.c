#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bfs.h"

Cell** create_matrix(int n, int m) {
	
	Cell** cells;
	int i, j;

	cells = (Cell**)calloc(n, sizeof(Cell*));
	assert(cells != NULL);

	for (i = 0; i < n; i++) {
		cells[i] = (Cell*)calloc(m, sizeof(Cell));
		assert(cells[i] != NULL);
		for (j = 0; j < m; j++) {
			cells[i][j].edges = (char*)calloc(MAX_EDGES + 1, sizeof(char));
			assert(cells[i][j].edges != NULL);
		}
	}

	return cells;
}

void deallocate_matrix(Cell** cells, int n, int m) {

	int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			free(cells[i][j].edges);
		}
		free(cells[i]);
	}

	return;
}

QueueStruct* create_queue() {

	QueueStruct* qs = (QueueStruct*)malloc(sizeof(QueueStruct));
	assert(qs != NULL);

	qs->size = QUEUE_BASE_SIZE;
	qs->queue = (CellCoords*)malloc(qs->size * sizeof(CellCoords));
	assert(qs->queue != NULL);
	qs->head = -1;
	qs->tail = -1;
	qs->n_elem = 0;

	return qs;
}

int is_queue_empty(QueueStruct* qs) {
	return (qs->n_elem == 0);
}

int is_queue_full(QueueStruct* qs) {
	return (qs->n_elem == qs->size);
}

void resize_queue(QueueStruct* qs) {

	CellCoords* new_queue;

	new_queue = (CellCoords*)malloc((qs->size) * 2 * sizeof(CellCoords));
	assert(new_queue != NULL);

	if (qs->head <= qs->tail) {
		memcpy(
			new_queue, 
			qs->queue + qs->head, 
			(qs->n_elem) * sizeof(CellCoords)
		);
	} else {
		memcpy(
			new_queue,
			qs->queue + qs->head,
			(qs->size - qs->head) * sizeof(CellCoords)
		);
		memcpy(
			new_queue + (qs->size - qs->head),
			qs->queue,
			(qs->tail + 1) * sizeof(CellCoords)
		);
	}

	free(qs->queue);
	qs->queue = new_queue;
	qs->head = 0;
	qs->tail = qs->n_elem - 1;
	qs->size = qs->size * 2;

	return;
}

void enqueue(QueueStruct* qs, CellCoords coords) {

	if (is_queue_full(qs)) {
		resize_queue(qs);
	}

	qs->tail = (qs->tail + 1) % qs->size;
	qs->queue[qs->tail] = coords;
	(qs->n_elem)++;

	return;
}

CellCoords dequeue(QueueStruct* qs) {

	CellCoords coords;

	coords = qs->queue[qs->head];
	qs->head = (qs->head + 1) % qs->size;
	(qs->n_elem)--;

	return coords;
}

void init_matrix(Cell** cells, int n, int m, FILE* input_file) {

	int read_char;
	int i = 0, j = 0;
	int p, q;
	int row_up_limit, row_down_limit, col_left_limit, col_right_limit;

	while ((read_char = fgetc(input_file)) != EOF) {

		if (read_char == '\n') {
			i++;
			j = 0;
			continue;
		}

		cells[i][j].element = (char)read_char;

		if (cells[i][j].element == OBSTACLE) {

			row_up_limit = -1;
			row_down_limit = 1;
			col_left_limit = -1;
			col_right_limit = 1;

			if (i == 0) {
				row_up_limit = 0;
			} else if (i == n-1) {
				row_down_limit = 0;
			}

			if (j == 0) {
				col_left_limit = 0;
			}

			if (j == m-1) {
				col_right_limit = 0;
			}

			for (p = row_up_limit; p <= row_down_limit; p++) {
				for (q = col_left_limit; q <= col_right_limit; q++) {
					cells[i+p][j+q].isWalkable = FALSE;
				}
			}
		} else if (cells[i][j].element == PATH && cells[i][j].isWalkable != FALSE) {
			if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
				cells[i][j].isWalkable = FALSE;
			} else {
				cells[i][j].isWalkable = TRUE;
			}
		}

		cells[i][j].distance = UNDEF_DIST;
		cells[i][j].status = NOT_VISITED;
		cells[i][j].sp_source_dir = UNDEF_DIR;

		j++;
	}

	if (ferror(input_file)) {
		fprintf(stderr, "\nAn error occurred while reading the file");
		exit(EXIT_FAILURE);
	}

	return;
}

CellCoords get_src_coords(CellCoords dst_coords, char direction) {

	CellCoords src_coords;

	switch (direction) {
		case NORD:
			src_coords.row = dst_coords.row + 1;
			src_coords.col = dst_coords.col;
			break;
		case EST:
			src_coords.row = dst_coords.row;
			src_coords.col = dst_coords.col - 1;
			break;
		case SUD:
			src_coords.row = dst_coords.row - 1;
			src_coords.col = dst_coords.col;
			break;
		case WEST:
			src_coords.row = dst_coords.row;
			src_coords.col = dst_coords.col + 1;
			break;
		default:
			fprintf(stderr, "\nInvalid direction");
			exit(EXIT_FAILURE);
	}

	return src_coords;
}

CellCoords get_dst_coords(CellCoords src_coords, char direction) {

	CellCoords dst_coords;

	switch (direction) {
		case NORD:
			dst_coords.row = src_coords.row - 1;
			dst_coords.col = src_coords.col;
			break;
		case EST:
			dst_coords.row = src_coords.row;
			dst_coords.col = src_coords.col + 1;
			break;
		case SUD:
			dst_coords.row = src_coords.row + 1;
			dst_coords.col = src_coords.col;
			break;
		case WEST:
			dst_coords.row = src_coords.row;
			dst_coords.col = src_coords.col - 1;
			break;
		default:
			fprintf(stderr, "\nInvalid direction");
			exit(EXIT_FAILURE);
	}

	return dst_coords;
}

void add_edge_if_walkable(Cell** cells, CellCoords src_coords, char direction, int* edges_count) {

	CellCoords dst_coords = get_dst_coords(src_coords, direction);

	if (cells[dst_coords.row][dst_coords.col].isWalkable == TRUE) {
		cells[src_coords.row][src_coords.col].edges[*edges_count] = direction;
		(*edges_count)++;
	}

	return;
}

void build_adjacency_lists(Cell** cells, int n, int m) {

	CellCoords coords;
	int i, j, e;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			if (cells[i][j].isWalkable == TRUE) {
				e = 0;
				coords.row = i;
				coords.col = j;
				add_edge_if_walkable(cells, coords, NORD, &e);
				add_edge_if_walkable(cells, coords, EST, &e);
				add_edge_if_walkable(cells, coords, SUD, &e);
				add_edge_if_walkable(cells, coords, WEST, &e);
			}
		}
	}

	return;
}

void bfs(Cell** cells) {

	CellCoords src_coords, dst_coords;
	int i;
	QueueStruct* qs = create_queue();

	src_coords.row = 1;
	src_coords.col = 1;
	qs->head = 0;
	enqueue(qs, src_coords);

	cells[1][1].distance = 0;
	cells[1][1].sp_source_dir = UNDEF_DIR;
	cells[1][1].status = NOT_ALL_ADJ_VISITED;

	while (!is_queue_empty(qs)) {

		src_coords = dequeue(qs);

		i = 0;
  		while (cells[src_coords.row][src_coords.col].edges[i] != NO_EDGE) {

			dst_coords = get_dst_coords(src_coords, cells[src_coords.row][src_coords.col].edges[i]);

			if (cells[dst_coords.row][dst_coords.col].status == NOT_VISITED) {
				cells[dst_coords.row][dst_coords.col].distance = cells[src_coords.row][src_coords.col].distance + 1;
				cells[dst_coords.row][dst_coords.col].sp_source_dir = cells[src_coords.row][src_coords.col].edges[i];
				cells[dst_coords.row][dst_coords.col].status = NOT_ALL_ADJ_VISITED;
				enqueue(qs, dst_coords);
			}

			i++;
		}

 		cells[src_coords.row][src_coords.col].status = ALL_ADJ_VISITED;
	}

	free(qs->queue);
	free(qs);

	return;
}

void print_shortest_path(Cell** cells, int n, int m) {

	CellCoords coords;
	char* shortest_path;
	char direction;
	int sp_length, i;

	sp_length = cells[n-2][m-2].distance;
	printf("%d\n", sp_length);

	if (sp_length != UNDEF_DIST) {

		shortest_path = (char*)malloc((sp_length + 1) * sizeof(char));
		assert(shortest_path != NULL);
		shortest_path[sp_length] = '\0';
		coords.row = n-2;
		coords.col = m-2;
		i = sp_length-1;

		while (coords.row != 1 || coords.col != 1) {
			direction = cells[coords.row][coords.col].sp_source_dir;
			shortest_path[i] = direction;
			coords = get_src_coords(coords, direction);
			i--;
		}

		printf("%s", shortest_path);

		free(shortest_path);
	}

	return;
}