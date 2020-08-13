#include "params.h"
#include <stdbool.h>

struct cell {
    int mined;
    int selected;
    int num_adjacent;
    struct cell * adjacent[MAX_ADJACENT];
    int coords[MAX_DIM];
    int hint;
};

int coordinates_to_index(int dim, int* dim_max, int* coords);

void index_to_coordinates(int index, int dim, int* dim_max, int* coords);

int calculate_num_cells(int dim, int* dim_sizes);

void calculate_size_of_dimensions(int dim, int* dim_sizes, int* maximums);

bool is_adjacent(int* this_cell, int dim, int* other_cell);

bool check_valid_coordinates(int dim, int* dim_sizes, int* coords);

void recursive_select(struct cell* cell);

bool check_win(struct cell* game, int num_cells);

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells);

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords);
