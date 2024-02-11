#ifndef BFS_H_INCLUDED
#define BFS_H_INCLUDED

#define NORD 'N'
#define SUD 'S'
#define EST 'E'
#define WEST 'W'

#define PATH '.'
#define OBSTACLE '*'

#define TRUE 1
#define UNDEF 0
#define FALSE -1

#define MAX_EDGES 4
#define NO_EDGE	'\0'

#define UNDEF_DIST -1
#define UNDEF_DIR '\0'

#define QUEUE_BASE_SIZE 50

typedef enum { 
	NOT_VISITED, 
	NOT_ALL_ADJ_VISITED, 
	ALL_ADJ_VISITED 
} CellStatus;

typedef struct Node {
	short int isWalkable;
	char element;
	char* edges;
	int distance;
	CellStatus status;
	char sp_source_dir;
} Cell;

typedef struct queue_element {
	int row;
	int col;
} CellCoords;

typedef struct queue_struct {
	CellCoords* queue;	
	int head;
	int tail;
	int size;
	int n_elem;
} QueueStruct;

Cell** create_matrix(int n, int m);

void deallocate_matrix(Cell** cells, int n, int m);

QueueStruct* create_queue();

int is_queue_empty(QueueStruct* qs);

int is_queue_full(QueueStruct* qs);

void resize_queue(QueueStruct* qs);

void enqueue(QueueStruct* qs, CellCoords coords);

CellCoords dequeue(QueueStruct* qs);

void init_matrix(Cell** cells, int n, int m, FILE* input_file);

CellCoords get_src_coords(CellCoords dst_coords, char direction);

CellCoords get_dst_coords(CellCoords src_coords, char direction);

void add_edge_if_walkable(Cell** cells, CellCoords src_coords, char direction, int* edges_count);

void build_adjacency_lists(Cell** cells, int n, int m);

void bfs(Cell** cells);

void print_shortest_path(Cell** cells, int n, int m);

#endif